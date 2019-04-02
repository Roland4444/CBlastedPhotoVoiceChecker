#ifndef LIBQC_LIBPQC_H
#define LIBQC_LIBPQC_H

#include "common.h"

#if defined(__cplusplus)
extern "C" {
#endif

enum CheckImageError {
    CheckImageReadConfiguration = LastPublicError,  // 102
    CheckImageIllegalProcessingEnvironment,  // 103
    CheckImageProcessingNotInitialized,  // 104
    CheckImageIllegalContent,  // 105
    CheckImageIllegalFormat,  // 106

    CheckImageGreatRecordLength,  // 107
    CheckImageCantDetectOneFace,  // 108

    CheckImageIllegalWidth              = 0x80000000,
    CheckImageIllegalHeight             = 0x40000000,
    CheckImageIllegalChannels           = 0x20000000,
    CheckImageIllegalDepth              = 0x10000000,
    CheckImageIllegalHeadPosY           = 0x08000000,
    CheckImageIllegalHeadPosX           = 0x04000000,
    CheckImageIllegalHeadPosZ           = 0x02000000,
    CheckImageIllegalEyeDistance        = 0x01000000,
    CheckImageIllegalFaceHorizontalPos  = 0x00800000,
    CheckImageIllegalFaceVerticalPos    = 0x00400000,
    CheckImageIllegalHeadHorizontalSize = 0x00200000,
    CheckImageIllegalHeadVerticalSize   = 0x00100000,
    CheckImageIllegalDetectionNearBorder = 0x00080000,
    CheckImageIllegalFaceTooSmall = 0x00040000,
    CheckImageIllegalMouth = 0x00020000,
    CheckImageIllegalMonochrome = 0x00010000,
    CheckImageIllegalFaceTooLight  = 0x00008000,
    CheckImageIllegalFaceTooDark = 0x00004000,
    CheckImageIllegalTooBlurry = 0x00002000,
};

bool SHARED_API i_create_session(struct Session *session, const char *configuration_file);
bool SHARED_API i_destroy_session(struct Session *session);
bool SHARED_API i_check_format(struct Session *session, const uint8_t *content, uint64_t content_size);
bool SHARED_API i_check(struct Session *session, const uint8_t *content, uint32_t width, uint32_t height, uint32_t stride, uint8_t depth);
bool SHARED_API i_result_session(struct Session *session, struct SessionValue **value);
SHARED_API char * i_session_configuration_version(struct Session *session);

#ifndef DISABLE_DEBUG_OUTPUT

#define OutputProcessingImageName "processing_image_name"

bool SHARED_API i_dbg_set_property(struct Session *session, const char *key, const char *value);

#endif

#if defined(__cplusplus)
}
#endif

#endif //LIBQC_LIBPQC_H
