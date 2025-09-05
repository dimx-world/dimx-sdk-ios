import {Utils} from 'dimx-cpp'

const Templates = []

function getTransformFromConfig(record) {
    return {
        position: record.pos,
        rotation_angles: record.rot,
        scale: record.scale_xyz ? record.scale_xyz : `${record.scale} ${record.scale} ${record.scale}`
    }
}

const FadeInActor = Object.freeze({
    type: 'Material',
    property: 'Alpha',
    duration: 1,
    track: {
        values: [0, 1]
    }
})

const DoubleSidedActor = Object.freeze({
    type: 'FaceCamera',
    fixed: true
})

const FaceCameraActor = Object.freeze({
    type: 'FaceCamera'
})

Templates.push({
    name: 'dummy',
    config: {
        name: null,
        pos: "0 0 0",
        rot: "0 0 0",
        scale_xyz: null,
        scale: 1,
        visible: true,
        analytics: false
    },
    handler: (parent, record) => {
        let config = {
            name: record.name,
            Node: {
                parent: parent.id(),
                transform: getTransformFromConfig(record),
                visible: record.visible
            },
            Dummy: {
                analytics: record.analytics
            }
        }
        parent.location().createObject(config)
    }
})

Templates.push({
    name: 'image',
    config: {
        image: "default",
        orient: "vert", // vert, horiz
        width: 1,
        height: 1,
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1,
        transparent: false,
        fade_in: true,
        flat: true,
        double_sided: false,
        face_camera: false,
        url: null,
        _meta_: {
            image: {
                resource: 'Texture'
            }
        }
    },
    handler: (parent, record) => {
        let config = {
            Node: {
                parent: parent.id(),
                transform: getTransformFromConfig(record)
            },
            ModelNode: {
                model: {
                    builder: {
                        type: "Plane",
                        plane: record.orient === 'vert' ? 'XY' : 'XZ',
                        width: record.width,
                        height: record.height,
                        origin: 'Center'
                    }
                },
                materials: {
                    ["1-0"]: {
                        base: "Standard",
                        transparent: record.transparent,
                        overrides: {
                            base_color_map: record.image,
                            flat: record.flat
                        }
                    }
                }
            }
        }

        parent.location().createObject(config, function (object) {
            if (record.url) {
                object.on('Click', function () {
                    Utils.openUrl(record.url)
                })
            }
            if (record.fade_in) {
                object.agents().add(FadeInActor);
            }
            if (record.face_camera) {
                object.agents().add(FaceCameraActor);
            } else {
                if (record.double_sided) {
                    object.agents().add(DoubleSidedActor);
                }
            }
        })
    }
})

Templates.push({
    name: 'model',
    config: {
        model: "box",
        anim: null,
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1,
        scale_xyz: null,
        transparent: false,
        fade_in: true,
        _meta_: {
            model: {
                resource: 'Model'
            }
        }
    },
    handler: (parent, record) => {
        let config = {
            Node: {
                parent: parent.id(),
                transform: getTransformFromConfig(record)
            },
            ModelNode: {
                model: record.model,
                animator: record.anim ? { start_animation: record.anim } : undefined
            }
        }

        if (record.transparent) {
            config.ModelNode.materials = { ["1-0"]:  { transparent: true } }
        }

        parent.location().createObject(config, function (object) {
            if (record.fade_in) {
                object.agents().add(FadeInActor);
            }
        })
    }
})

Templates.push({
    name: 'audio',
    config: {
        name: null,
        audio: '',
        cover_image: '',
        cover_alpha: 1.0,
        ui_always_on: false,
        style: "Circle", // Flat, Circle
        progress_text: false,
        width: 0.5,
        height: 0.5,
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1,
        autoplay: false,
        loop: false,
        sound_3d: false,
        sound_ref_dist: 5,
        sound_max_dist: 15,
        fade_in: true,
        double_sided: false,
        face_camera: false,
        _meta_: {
            audio: {
                resource: 'File'
            },
            cover_image: {
                resource: 'Texture'
            }
        }
    },
    handler: (parent, record) => {
        let config = {
            name: record.name ? record.name : undefined,
            Node: {
                parent: parent.id(),
                transform: getTransformFromConfig(record)
            },
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
            }
        }

        let callback = undefined
        if (record.fade_in || record.face_camera || record.double_sided) {
            callback = (object) => {
                if (record.fade_in) {
                    object.agents().add(FadeInActor);
                }
                if (record.face_camera) {
                    object.agents().add(FaceCameraActor);
                } else {
                    if (record.double_sided) {
                        object.agents().add(DoubleSidedActor);
                    }
                }
            }
        }

        parent.location().createObject(config, callback)
    }
})

Templates.push({
    name: 'video',
    config: {
        name: null,
        video: null,
        width: 1.28,
        height: 0.72,
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1,
        autoplay: false,
        loop: false,
        sound_3d: false,
        sound_ref_dist: 5,
        sound_max_dist: 15,
        fade_in: true,
        double_sided: false,
        face_camera: false,
        alpha_matte: false,
        _meta_: {
            video: {
                resource: 'File'
            }
        }
    },
    handler: (parent, record) => {
        let config = {
            name: record.name ? record.name : undefined,
            Node: {
                parent: parent.id(),
                transform: getTransformFromConfig(record)
            },
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
            }
        }

        let callback = undefined
        if (record.fade_in || record.face_camera || record.double_sided) {
            callback = (object) => {
                if (record.fade_in) {
                    object.agents().add(FadeInActor);
                }
                if (record.face_camera) {
                    object.agents().add(FaceCameraActor);
                } else {
                    if (record.double_sided) {
                        object.agents().add(DoubleSidedActor);
                    }
                }
            }
        }

        parent.location().createObject(config, callback)
    }
})

Templates.push({
    name: 'text2d',
    config: {
        text: 'Text',
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1,

        font_size: 20,
        frame_padding: 0,
        frame_width: 0,
        frame_height: 0,
        border_size: 0,
        corner_radius: 0,

        text_color: '1 1 1 1',
        background_color: '0 0 0 0',
        border_color: '1 1 1 1',

        fade_in: true,
        flat: true,
        double_sided: false,
        face_camera: false
    },
    handler: (parent, record) => {
        let config = {
            Node: {
                parent: parent.id(),
                transform: getTransformFromConfig(record)
            },
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
            }
        }

        let callback = undefined
        if (record.fade_in || record.double_sided || record.face_camera) {
            callback = (object) => {
                if (record.fade_in) {
                    object.agents().add(FadeInActor);
                }
                if (record.face_camera) {
                    object.agents().add(FaceCameraActor);
                } else {
                    if (record.double_sided) {
                        object.agents().add(DoubleSidedActor);
                    }
                }
            }
        }

        parent.location().createObject(config, callback)
    }
})

Templates.push({
    name: 'trigger',
    config: {
        pos: "0 0 0",
        radius: 3,
        once: false,
        on_enter: null,
        on_exit: null
    },
    handler: (parent, record) => {
        let config = {
            Node: {
                parent: parent.id(),
                transform: {
                    position: record.pos
                }
            },
            Trigger: {
                radius: record.radius,
                once: record.once,
                on_enter: record.on_enter,
                on_exit: record.on_exit
            }
        }
        parent.location().createObject(config)
    }
})

export { Templates };