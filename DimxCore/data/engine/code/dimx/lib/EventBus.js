export class EventBus
{
    listeners = {}

    subscribe(key, listener) {
        if (!key) {
            console.error('BusEvent: invalid key to subscribe')
            return
        }

        if (Array.isArray(key)) {
            if (key.length != 2 || !(key[0] && key[1])) {
                console.eror(`BusEvent: subscribe supports only arrays of two elements. Received: ${JSON.stringify(key)}`)
                return
            }

            let eventRecord = this.listeners[key[0]]
            if (!eventRecord) {
                eventRecord = {}
                this.listeners[key[0]] = eventRecord
            }
            if (!eventRecord.map) {
                eventRecord.map = {}
            }
            let keyRecord = eventRecord.map[key[1]]
            if (!keyRecord) {
                keyRecord = []
                eventRecord.map[key[1]] = keyRecord
            }
            keyRecord.push(listener)
        } else {
            let eventRecord = this.listeners[key]
            if (!eventRecord) {
                eventRecord = {}
                this.listeners[key] = eventRecord
            }
            if (!eventRecord.list) {
                eventRecord.list = []
            }
            eventRecord.list.push(listener)
        }
    }

    unsubscribe() {
        //...
    }

    publish(key, ...args) {
        if (!key) {
            console.error('BusEvent: invalid key to publish')
            return
        }

        if (Array.isArray(key)) {
            if (key.length != 2 || !key[0]) {
                console.eror(`BusEvent: publish supports only arrays of two elements. Received: ${JSON.stringify(key)}`)
                return
            }
            let eventRecord = this.listeners[key[0]]
            if (!eventRecord) {
                return
            }
            if (key[1] && eventRecord.map) {
                let keyRecord = eventRecord.map[key[1]]
                if (keyRecord) {
                    for (const listener of keyRecord) {
                        listener(...args)
                    }
                }
            }
            if (eventRecord.list) {
                for (const listener of eventRecord.list) {
                    listener(...args)
                }
            }
        } else {
            let eventRecord = this.listeners[key]
            if (eventRecord && eventRecord.list) {
                if (eventRecord.list) {
                    for (const listener of eventRecord.list) {
                        listener(...args)
                    }
                }
            }
        }
    }
}
