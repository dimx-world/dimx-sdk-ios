export const ContentTemplates = {
    image: {
        image: "default",
        orient: "vert", // vert, horiz
        side: "front", // front, back, both
        width: 1,
        height: 1,
        pos: "0 0 0",
        rot: "0 0 0",
        transparent: false,
        animate: true,
        flat: true,
        url: null,
        _props_: {
            image: {
                resource: 'Texture'
            }
        }
    },

    model: {
        model: "box",
        anim: null,
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1, // uniform scale in all dimensions
        scale_xyz: null, // scale by dimension "x y z"
        transparent: false,
        animate: true,
        _props_: {
            model: {
                resource: 'Model'
            }
        }
    },

    video: {
        name: null,
        video: null,
        width: 1.28,
        height: 0.72,
        pos: "0 0 0",
        rot: "0 0 0",
        scale: 1,
        autoplay: false,
        loop: false,
        sound_3d: false,     // enable stereo sound
        sound_ref_dist: 5,   // 'reference' distance of max volume for 3D sounds.
                             // The volume will start to decrease after this distance.
        sound_max_dist: 15,  // Beyond this distance 3D sounds won't be heard
        animate: true,
        face_camera: false,
        alpha_matte: false,
        _props_: {
            video: {
                resource: 'File'
            }
        }
    },

    audio: {
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
        sound_3d: false,     // same as in video
        sound_ref_dist: 5,
        sound_max_dist: 15,
        animate: true,
        face_camera: false,
        _props_: {
            audio: {
                resource: 'File'
            },
            cover_image: {
                resource: 'Texture'
            }
        }
    },

    dummy: {
        name: null,
        pos: "0 0 0",
        rot: "0 0 0",
        scale_xyz: null,
        scale: 1,
        visible: true,
        analytics: false
    },

    trigger: {
        pos: "0 0 0",
        radius: 3,
        once: false,
        on_enter: null,
        on_exit: null
    },

    text2d: {
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

        animate: true,
        flat: true
    }
}
