import {ContentTemplates} from './ContentTemplates'
import * as ContentHandlers from './ContentHandlers'

export class ContentFactory {
    parent = undefined
    templates = {}
    mapping = {}
    handlers = {}

    constructor(parent) {
        this.parent = parent

        // default stuff
        this.addTemplates(ContentTemplates)

        this.addHandler('image', ContentHandlers.handleImageContent)
        this.addHandler('model', ContentHandlers.handleModelContent)
        this.addHandler('video', ContentHandlers.handleVideoContent)
        this.addHandler('audio', ContentHandlers.handleAudioContent)
        this.addHandler('dummy', ContentHandlers.handleDummyContent)
        this.addHandler('trigger', ContentHandlers.handleTriggerContent)
        this.addHandler('text2d', ContentHandlers.handleText2dContent)
    }

    getTemplate(key) {
        let tmpl = this.templates[key]
        if (!tmpl && this.parent) {
            tmpl = this.parent.getTemplate(key)
        }
        return tmpl
    }

    getHandler(key) {
        let handler = this.handlers[key]
        if (!handler && this.parent) {
            handler = this.parent.getHandler(key)
        }
        return handler
    }

    expandTemplate(record, inputTemplates, visited) {
        if (record.T in visited) {
            console.error(`Circular dependency detected: template [${JSON.stringify(record)}] visited [${JSON.stringify(visited)}]`)
            return null
        }
        visited[record.T] = true

        let baseTmpl = this.getTemplate(record.T)
        if (baseTmpl) {
            return {...baseTmpl, ...record, ...{T: baseTmpl.T}}
        }

        if (record.T in inputTemplates) {
            let baseTmpl = inputTemplates[record.T]
            return this.expandTemplate({...baseTmpl, ...record, ...{T: baseTmpl.T}}, inputTemplates, visited)
        }

        console.error(`Unknown base template [${record.T}]`)
        return null
    }

    validateTemplate(record) {
        let globalRecord = this.getTemplate(record.T)
        if (!globalRecord) {
            console.error(`Unknown template [${record.T}]`)
            return false
        }
        for (const key of Object.keys(record)) {
            if (key[0] == '_') {
                continue
            }
            if (!(key in globalRecord)) {
                console.error(`Unknown template field [${key}]`)
                return false
            }
        }
        return true
    }

    addTemplates(inputTemplates) {
        if (!inputTemplates) {
            console.error(`Invalid templates being added [${inputTemplates}]`)
            return
        }

        for (const [key, record] of Object.entries(inputTemplates)) {
            if (!record) {
                console.error(`Null template [${key}]. Ignoring`)
                continue;
            }
            if (key in this.templates) {
                console.error(`Template [${key}] already defined. Ignoring the duplicate`)
                continue;
            }
            let expandedRec = null
            if (record.T) {
                expandedRec = this.expandTemplate(record, inputTemplates, {})
                if (!expandedRec) {
                    console.error(`Failed to expand content template [${key}] [${JSON.stringify(record)}]`)
                    continue
                }
                if (!this.validateTemplate(expandedRec)) {
                    console.error(`Invalid content template [${key}] [${JSON.stringify(record)}]`)
                    continue
                }
            } else {
                // global template
                expandedRec = record
                expandedRec.T = key
            }

            this.templates[key] = expandedRec;
        }
    }

    expandRecord(record, tmpl) {
        if (!tmpl) {
            console.error(`Unknown item template [${record.T}]`)
            return null
        }

        for (const key of Object.keys(record)) {
            if (key[0] != '_' && !(key in tmpl)) {
                console.error(`Invalid content item [${JSON.stringify(record)}]`)
                return null
            }
        }
        return {...tmpl, ...record, ...{T: tmpl.T}}
    }

    addMapping(content) {
        if (!content || !Array.isArray(content.records)) {
            console.error(`Invalid content being added [${JSON.stringify(content)}]`)
            return
        }
        for (const record of content.records) {
            let recordKey = record.D
            delete record.D
            if (!(recordKey in this.mapping)) {
                this.mapping[recordKey] = []
            }
            record._assets_ = content.assets
            this.mapping[recordKey].push(record)
        }
    }

    addHandler(key, callback) {
        this.handlers[key] = callback
    }

    createContent(object) {
        let array = this.mapping[object.name()]
        if (!array) {
            return false
        }
        let result = false
        for (let record of array) {
            const tmpl = this.getTemplate(record.T)
            if (!tmpl) {
                console.warn(`Unknown template [${record.T}] for object [${object.name()}]`)
                continue
            }
            let handler = this.getHandler(tmpl.T)
            if (!handler) {
                console.warn(`Unknown content handler [${tmpl.T}] for object [${object.name()}]`)
                continue
            }
            record = this.expandRecord(record, tmpl)
            if (record) {
                if (this.finalizeContentRecord(record, object)) {
                    result = true
                    handler(object.location(), object.id(), record)
                }
            }
        }
        return result
    }

    finalizeContentRecord(record, object) {
        if (record._props_) {
            for (const [key, value] of Object.entries(record._props_)) {
                let prop = record[key]
                if (prop == null) {
                    console.error(`Invalid _prop_ [${key}] defined in template [${record.T}]`)
                    return false
                }

                if (value.resource) {
                    if (!prop) {
                        continue
                    }
                    if (object.location().validateResource(value.resource, prop)) {
                        continue
                    }

                    let valid = false

                    if (record._assets_) {
                        if (!Array.isArray(record._assets_)) {
                            console.error('Invalid assets node in content')
                            return false
                        }
                        for (const path of record._assets_) {
                            if (object.location().validateResource(value.resource, `${path}/${prop}`)) {
                                record[key] = `${path}/${prop}`
                                valid = true
                                break
                            }
                        }
                    }

                    if (!valid) {
                        console.error(`Unable to locate content file: [${prop}]`)
                        return false
                    }
                }
            }
            delete record._props_
        }
        return true
    }
}
