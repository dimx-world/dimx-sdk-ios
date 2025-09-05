import {Dimension} from 'dimx-cpp'
import {Templates} from './ContentTemplates'

export class ContentFactory {
    templates = {}
    content = {}

    constructor() {
        for (const tmpl of Templates) {
            this.addTemplate(tmpl.name, tmpl.config, tmpl.handler)
        }
    }

    expandTemplate(record, visited) {
        if (!record.T) {
            return record
        }

        if (record.T in visited) {
            console.error(`Circular dependency detected: template [${JSON.stringify(record)}] visited [${JSON.stringify(visited)}]`)
            return null
        }
        visited[record.T] = true

        let baseTmpl = this.templates[record.T]
        if (!baseTmpl) {
            console.error(`Unknown base template [${record.T}]`)
            return null
        }

        for (const key of Object.keys(record)) {
            if (key[0] === '_' || key === 'T') {
                continue
            }

            const baseField = baseTmpl[key]
            if (baseField === undefined) {
                console.error(`Unknown field [${key}] in template derived from [${record.T}]`)
                return null
            }
        }

        return {...baseTmpl, ...record}
    }

    addTemplate(name, template, handler) {
        if (!name || !template /*|| !handler*/) {
            console.error(`Null template [${name}]. Ignoring`)
            return;
        }
        if (name in this.templates) {
            console.error(`Template [${name}] already defined. Ignoring the duplicate`)
            return;
        }
        let expandedRec = this.expandTemplate(template, {})
        if (!expandedRec) {
            console.error(`Failed to expand content template [${name}] [${JSON.stringify(template)}]`)
            return;
        }

        if (handler) {
            expandedRec._handler_ = handler
        }

        this.templates[name] = expandedRec;
    }

    addContent(cluster, content) {
        if (!content || !Array.isArray(content)) {
            console.error(`Invalid content being added [${JSON.stringify(content)}]`)
            return
        }
        for (const record of content) {
            let recordKey = record.D
            delete record.D
            if (!(recordKey in this.content)) {
                this.content[recordKey] = []
            }
            record._cluster_ = cluster
            this.content[recordKey].push(record)
        }
    }

    expandRecord(record, tmpl) {
        if (!tmpl) {
            console.error(`Unknown item template [${record.T}]`)
            return null
        }

        for (const key of Object.keys(record)) {
            if (key[0] != '_' && key != 'T' && !(key in tmpl)) {
                console.error(`Invalid content item [${JSON.stringify(record)}]`)
                return null
            }
        }
        const expanded = {...tmpl, ...record}

        if (expanded._meta_) {
            for (const [key, info] of Object.entries(expanded._meta_)) {
                if (!info.resource) {
                    continue
                }
                const value = expanded[key];
                if (!value) {
                    continue
                }

                if (!expanded?._cluster_?.basePath) {
                    continue
                }

                const pathInCluster = `${expanded._cluster_.basePath()}/${value}`;
                if (Dimension.validateResource(info.resource, pathInCluster)) {
                    expanded[key] = pathInCluster
                } else if (!Dimension.validateResource(info.resource, value)) {
                    console.error(`Unable to locate content resource [${info.resource}] [${value}]`)
                    return null
                }
            }
        }

        delete expanded._meta_;
        delete expanded._cluster_;
        return expanded
    }

    createContent(object) {
        let array = this.content[object.name()]
        if (!array) {
            return
        }
        for (let record of array) {
            const tmpl = this.templates[record.T]
            if (!tmpl) {
                console.warn(`Unknown template [${record.T}] for object [${object.name()}]`)
                continue
            }

            if (!tmpl._handler_) {
                console.warn(`Unknown content handler [${tmpl.T}] for object [${object.name()}]`)
                continue
            }

            record = this.expandRecord(record, tmpl)
            if (!record) {
                continue
            }

            tmpl._handler_(object, record)
        }
    }

}
