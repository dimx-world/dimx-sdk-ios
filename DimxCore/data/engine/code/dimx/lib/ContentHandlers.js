import {Utils} from 'dimx-cpp'

const AlphaActor = Object.freeze({
    type: 'Material',
    property: 'Alpha',
    duration: 1,
    track: {
        values: [0, 1]
    }
})

const FaceCameraActor = Object.freeze({
    type: 'FaceCamera'
})

function animateAlpha(object) {
    object.agents().add(AlphaActor);
}

export function createSidedImage(location, parentId, record, side) {
    let rot = record.rot

    if (side == 'back') {
        let numbers = rot.split(' ').map(Number);
        if (numbers.length == 3) {
            if (record.orient == 'vert') {
                numbers[1] += 180
            } else {
                numbers[0] += 180
            }
            rot = numbers.join(' ')
        }
    }

    let imageRotX = record.orient == 'vert' ? 90 : 0

    let config = [
        {
            Node: {
                parent: parentId,
                transform: {
                    position: record.pos,
                    rotation_angles: rot
                }
            }
        },
        {
            ModelNode: {
                materials: {
                    ["1-0"]: {
                        base: "Standard",
                        overrides: {
                            base_color_map: record.image,
                            flat: record.flat
                        }
                    }
                },
                model: "plane"
            },
            Node: {
                transform: {
                    rotation_angles: `${imageRotX} 0 0`,
                    scale: `${record.width} 1 ${record.height}`
                }
            }
        }
    ]

    if (record.transparent) {
        config[1].ModelNode.materials["1-0"].transparent = true
    }

    location.createObject(config, function (objects) {
        if (record.url) {
            objects[1].on('Click', function () {
                Utils.openUrl(record.url)
            })
        }
        if (record.animate) {
            objects[1].agents().add(AlphaActor);
        }
    })
}

export function handleImageContent(location, parentId, record) {
    if (record.side == "both") {
        createSidedImage(location, parentId, record, 'front')
        createSidedImage(location, parentId, record, 'back')
    } else {
        createSidedImage(location, parentId, record, record.side)
    }
}

export function handleModelContent(location, parentId, record) {
    let config = {
        ModelNode: {
            model: record.model,
            animator: record.anim ? { start_animation: record.anim } : undefined
        },
        Node: {
            parent: parentId,
            transform: {
                position: record.pos,
                rotation_angles: record.rot,
                scale: record.scale_xyz ? record.scale_xyz : `${record.scale} ${record.scale} ${record.scale}`
            }
        }
    }

    if (record.transparent) {
        config.ModelNode.materials = { ["1-0"]:  { transparent: true } }
    }

    location.createObject(config, record.animate ? animateAlpha : undefined)
}

export function handleVideoContent(location, parentId, record) {
    let config = {
        name: record.name ? record.name : undefined,
        VideoPlayer: {
            filename: record.video ? record.video : '',
            width: record.width,
            height: record.height,
            autoplay: record.autoplay,
            loop: record.loop,
            sound_3d: record.sound_3d,
            sound_ref_dist: record.sound_ref_dist,
            sound_max_dist: record.sound_max_dist,
            alpha_matte: record.alpha_matte
        },
        Node: {
            parent: parentId,
            transform: {
                position: record.pos,
                rotation_angles: record.rot,
                scale: `${record.scale} ${record.scale} ${record.scale}`
            }
        }
    }

    let callback = undefined
    if (config, record.animate || record.face_camera) {
        callback = (object) => {
            if (record.animate) {
                object.agents().add(AlphaActor);
            }
            if (record.face_camera) {
                object.agents().add(FaceCameraActor);
            }
        }
    }

    location.createObject(config, callback)
}

export function handleAudioContent(location, parentId, record) {
    let config = {
        name: record.name ? record.name : undefined,
        AudioPlayer: {
            filename: record.audio,
            ui_always_on: record.ui_always_on,
            style: record.style,
            cover_image: record.cover_image,
            cover_alpha: record.cover_alpha,
            progress_text: record.progress_text,
            width: record.width,
            height: record.height,
            autoplay: record.autoplay,
            loop: record.loop,
            sound_3d: record.sound_3d,
            sound_ref_dist: record.sound_ref_dist,
            sound_max_dist: record.sound_max_dist
        },
        Node: {
            parent: parentId,
            transform: {
                position: record.pos,
                rotation_angles: record.rot,
                scale: `${record.scale} ${record.scale} ${record.scale}`
            }
        }
    }

    let callback = undefined
    if (config, record.animate || record.face_camera) {
        callback = (object) => {
            if (record.animate) {
                object.agents().add(AlphaActor);
            }
            if (record.face_camera) {
                object.agents().add(FaceCameraActor);
            }
        }
    }

    location.createObject(config, callback)
}

export function handleDummyContent(location, parentId, record) {
    let config = {
        name: record.name ? record.name : undefined,
        Dummy: {
            analytics: record.analytics
        },
        Node: {
            parent: parentId,
            visible: record.visible,
            transform: {
                position: record.pos,
                rotation_angles: record.rot,
                scale: record.scale_xyz ? record.scale_xyz : `${record.scale} ${record.scale} ${record.scale}`
            }
        }
    }
    location.createObject(config)
}

export function handleTriggerContent(location, parentId, record) {
    let config = {
        Trigger: {
            radius: record.radius,
            once: record.once,
            on_enter: record.on_enter,
            on_exit: record.on_exit
        },
        Node: {
            parent: parentId,
            transform: {
                position: record.pos
            }
        }
    }
    location.createObject(config)
}

export function handleText2dContent(location, parentId, record) {
    let config = {
        ModelNode: {
            model: {
                builder: {
                    type: "Text2D",
                    text: record.text,
                    font_size: record.font_size,
                    frame_padding: record.frame_padding,
                    frame_width: record.frame_width,
                    frame_height: record.frame_height,
                    border_size: record.border_size,
                    corner_radius: record.corner_radius,
                    text_color: record.text_color,
                    background_color: record.background_color,
                    border_color: record.border_color,
                    flat_material: record.flat
                }
            }
        },
        Node: {
            parent: parentId,
            transform: {
                position: record.pos,
                rotation_angles: record.rot,
                scale: `${record.scale} ${record.scale} ${record.scale}`
            }
        }
    }

    location.createObject(config, record.animate ? animateAlpha : undefined)

}
