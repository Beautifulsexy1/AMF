// 
// Notice Regarding Standards.  AMD does not provide a license or sublicense to
// any Intellectual Property Rights relating to any standards, including but not
// limited to any audio and/or video codec technologies such as MPEG-2, MPEG-4;
// AVC/H.264; HEVC/H.265; AAC decode/FFMPEG; AAC encode/FFMPEG; VC-1; and MP3
// (collectively, the "Media Technologies"). For clarity, you will pay any
// royalties due for such third party technologies, which may include the Media
// Technologies that are owed as a result of AMD providing the Software to you.
// 
// MIT license 
// 
// Copyright (c) 2018 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "EncoderParamsHEVC.h"

#include "public/include/components/VideoEncoderHEVC.h"
#include "public/samples/CPPSamples/common/CmdLogger.h"

#include <algorithm>
#include <iterator>
#include <cctype>

static AMF_RESULT ParamConverterUsageHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_USAGE_ENUM paramValue;

    std::wstring uppValue = toUpper(value);
    if(uppValue == L"TRANSCODING" || uppValue == L"0")
    {
        paramValue = AMF_VIDEO_ENCODER_HEVC_USAGE_TRANSCONDING;
    } 
    else if(uppValue == L"ULTRALOWLATENCY"|| uppValue == L"1") 
    {
        paramValue = AMF_VIDEO_ENCODER_HEVC_USAGE_ULTRA_LOW_LATENCY;
    } 
    else if(uppValue == L"LOWLATENCY"|| uppValue == L"2") 
    {
        paramValue = AMF_VIDEO_ENCODER_HEVC_USAGE_LOW_LATENCY;
    } 
    else if(uppValue == L"WEBCAM"|| uppValue == L"3") 
    {
        paramValue = AMF_VIDEO_ENCODER_HEVC_USAGE_WEBCAM;
    } 
    else {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_USAGE_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}
static AMF_RESULT ParamConverterQualityHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_ENUM paramValue;

    std::wstring uppValue = toUpper(value);
    if(uppValue == L"BALANCED" || uppValue== L"0")
    {
        paramValue = AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_BALANCED;
    } else if(uppValue == L"SPEED" || uppValue== L"1") {
        paramValue = AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_SPEED;
    } else if(uppValue == L"QUALITY" || uppValue== L"2") {
        paramValue = AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_QUALITY;
    } else {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}
static AMF_RESULT ParamConverterProfileHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_PROFILE_ENUM paramValue;
    std::wstring uppValue = toUpper(value);
    if(uppValue == L"MAIN"|| uppValue == L"1") 
    {   // only main profile is supported for HEVC encoder
        paramValue = AMF_VIDEO_ENCODER_HEVC_PROFILE_MAIN;
    }  else 
    {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_PROFILE_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterTierHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_TIER_ENUM paramValue;
    std::wstring uppValue = toUpper(value);
    if(uppValue == L"MAIN"|| uppValue == L"0") 
    {   
        paramValue = AMF_VIDEO_ENCODER_HEVC_TIER_MAIN;
    } else if(uppValue == L"HIGH"|| uppValue == L"1") 
    {   
       paramValue = AMF_VIDEO_ENCODER_HEVC_TIER_HIGH;
    }  else 
    {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_TIER_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}


static AMF_RESULT ParamConverterProfileLevelHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_LEVEL_ENUM paramValue;
    std::wstring uppValue = toUpper(value);
    if(uppValue == L"1.0" || uppValue == L"10" || uppValue == L"1" || uppValue == L"30"){
        paramValue = AMF_LEVEL_1;
    }else if(uppValue == L"2.0" || uppValue == L"20" || uppValue == L"2" || uppValue == L"60"){
        paramValue = AMF_LEVEL_2;
    }else if(uppValue == L"2.1" || uppValue == L"21" || uppValue == L"63"){
        paramValue = AMF_LEVEL_2_1;
    }else if(uppValue == L"3.0" || uppValue == L"30" || uppValue == L"3" || uppValue == L"90"){
        paramValue = AMF_LEVEL_3;
    }else if(uppValue == L"3.1" || uppValue == L"31" || uppValue == L"93"){
        paramValue = AMF_LEVEL_3_1;
    }else if(uppValue == L"4.0" || uppValue == L"40" || uppValue == L"4" || uppValue == L"120"){
        paramValue = AMF_LEVEL_4;
    }else if(uppValue == L"4.1" || uppValue == L"41" || uppValue == L"123"){
        paramValue = AMF_LEVEL_4_1;
    }else if(uppValue == L"5.0" || uppValue == L"50" || uppValue == L"5" || uppValue == L"150"){
        paramValue = AMF_LEVEL_5;
    }else if(uppValue == L"5.1" || uppValue == L"51" || uppValue == L"153"){
        paramValue = AMF_LEVEL_5_1;
    }else if(uppValue == L"5.2" || uppValue == L"52" || uppValue == L"156"){
        paramValue = AMF_LEVEL_5_2;
    }else if(uppValue == L"6.0" || uppValue == L"60" || uppValue == L"6" || uppValue == L"180"){
        paramValue = AMF_LEVEL_6;
    }else if(uppValue == L"6.1" || uppValue == L"61" || uppValue == L"183"){
        paramValue = AMF_LEVEL_6_1;
    }else if(uppValue == L"6.2" || uppValue == L"62" || uppValue == L"186"){
        paramValue = AMF_LEVEL_6_2;
    } else {
        LOG_ERROR(L"ProfileLevel hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterRateControlHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_ENUM paramValue;
    std::wstring uppValue = toUpper(value);
    if(uppValue == L"CQP" || uppValue == L"0")
    {
        paramValue = AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_CONSTANT_QP;
    } else if(uppValue == L"VBR_LAT" || uppValue == L"1") {
        paramValue = AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_LATENCY_CONSTRAINED_VBR;
    } else if(uppValue == L"VBR" || uppValue == L"2") {
        paramValue = AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_PEAK_CONSTRAINED_VBR;
    } else if(uppValue == L"CBR" || uppValue == L"3") {
        paramValue = AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD_CBR;
    } else {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}


static AMF_RESULT ParamConverterPictureTypeHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_ENUM paramValue;
    std::wstring uppValue = toUpper(value);
    if(uppValue == L"NONE" || uppValue == L"0")
    {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_NONE;
    } else if(uppValue == L"SKIP" || uppValue == L"1") {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_SKIP;
    } else if(uppValue == L"IDR" || uppValue == L"2") {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_IDR;
    } else if(uppValue == L"I" || uppValue == L"3") {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_I;
    } else if(uppValue == L"P" || uppValue == L"4") {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_P;
    } else {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_PICTURE_TYPE_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterInsertionModeHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE_ENUM paramValue;
    std::wstring uppValue = toUpper(value);
    if(uppValue == L"NONE" || uppValue == L"0")
    {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE_NONE;
    } else if(uppValue == L"GOP" || uppValue == L"1") {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE_GOP_ALIGNED;
    } else if(uppValue == L"IDR" || uppValue == L"2") {
        paramValue =  AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE_IDR_ALIGNED;
    } else {
        LOG_ERROR(L"AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterSceneChangeHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_PA_SCENE_CHANGE_DETECTION_SENSITIVITY_ENUM paramValue;

    std::wstring uppValue = toUpper(value);
    if (uppValue == L"LOW" || uppValue == L"0")
    {
        paramValue = AMF_PA_SCENE_CHANGE_DETECTION_SENSITIVITY_LOW;
    }
    else if (uppValue == L"MEDIUM" || uppValue == L"1") {
        paramValue = AMF_PA_SCENE_CHANGE_DETECTION_SENSITIVITY_MEDIUM;
    }
    else if (uppValue == L"HIGH" || uppValue == L"2") {
        paramValue = AMF_PA_SCENE_CHANGE_DETECTION_SENSITIVITY_HIGH;
    }
    else {
        LOG_ERROR(L"AMF_PA_SCENE_CHANGE_DETECTION_SENSITIVITY_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterStaticSceneHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_PA_STATIC_SCENE_DETECTION_SENSITIVITY_ENUM paramValue;

    std::wstring uppValue = toUpper(value);
    if (uppValue == L"LOW" || uppValue == L"0")
    {
        paramValue = AMF_PA_STATIC_SCENE_DETECTION_SENSITIVITY_LOW;
    }
    else if (uppValue == L"MEDIUM" || uppValue == L"1") {
        paramValue = AMF_PA_STATIC_SCENE_DETECTION_SENSITIVITY_MEDIUM;
    }
    else if (uppValue == L"HIGH" || uppValue == L"2") {
        paramValue = AMF_PA_STATIC_SCENE_DETECTION_SENSITIVITY_HIGH;
    }
    else {
        LOG_ERROR(L"AMF_PA_STATIC_SCENE_DETECTION_SENSITIVITY_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterActivityTypeHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_PA_ACTIVITY_TYPE_ENUM paramValue;

    std::wstring uppValue = toUpper(value);
    if (uppValue == L"Y")
    {
        paramValue = AMF_PA_ACTIVITY_Y;
    }
    else if (uppValue == L"YUV") {
        paramValue = AMF_PA_ACTIVITY_YUV;
    }
    else {
        LOG_ERROR(L"AMF_PA_ACTIVITY_TYPE_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}

static AMF_RESULT ParamConverterCAQStrengthHEVC(const std::wstring& value, amf::AMFVariant& valueOut)
{
    AMF_PA_CAQ_STRENGTH_ENUM paramValue;

    std::wstring uppValue = toUpper(value);
    if (uppValue == L"LOW" || uppValue == L"0")
    {
        paramValue = AMF_PA_CAQ_STRENGTH_LOW;
    }
    else if (uppValue == L"MEDIUM" || uppValue == L"1") {
        paramValue = AMF_PA_CAQ_STRENGTH_MEDIUM;
    }
    else if (uppValue == L"HIGH" || uppValue == L"2") {
        paramValue = AMF_PA_CAQ_STRENGTH_HIGH;
    }
    else {
        LOG_ERROR(L"AMF_PA_CAQ_STRENGTH_ENUM hasn't \"" << value << L"\" value.");
        return AMF_INVALID_ARG;
    }
    valueOut = amf_int64(paramValue);
    return AMF_OK;
}


AMF_RESULT RegisterEncoderParamsHEVC(ParametersStorage* pParams)
{
    pParams->SetParamDescription(SETFRAMEPARAMFREQ_PARAM_NAME, ParamCommon, L"Frequency of applying frame parameters (in frames, default = 0 )", ParamConverterInt64);
    pParams->SetParamDescription(SETDYNAMICPARAMFREQ_PARAM_NAME, ParamCommon, L"Frequency of applying dynamic parameters. (in frames, default = 0 )", ParamConverterInt64);


    // ------------- Encoder params usage---------------
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_USAGE, ParamEncoderUsage, L"Encoder usage type. Set many default parameters. (TRANSCONDING, ULTRALOWLATENCY, LOWLATENCY, WEBCAM, default = N/A)", ParamConverterUsageHEVC);
    // ------------- Encoder params static---------------
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_PROFILE, ParamEncoderStatic, L"HEVC profile (Main, default = Main", ParamConverterProfileHEVC);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_TIER, ParamEncoderStatic, L"HEVC tier (Main, High, default = Main", ParamConverterTierHEVC);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_PROFILE_LEVEL, ParamEncoderStatic, L"HEVC profile level (float or integer, default = based on HW", ParamConverterProfileLevelHEVC);

    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_QUALITY_PRESET, ParamEncoderStatic, L"Quality Preset (BALANCED, SPEED, QUALITY default = depends on USAGE)", ParamConverterQualityHEVC);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MAX_LTR_FRAMES, ParamEncoderStatic, L"Max Of LTR frames (integer, default = 0)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MAX_NUM_REFRAMES, ParamEncoderStatic,  L" Maximum number of reference frames default = 1", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_ENABLE_VBAQ, ParamEncoderStatic, L"Enable VBAQ(true, false default =  false)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_HIGH_MOTION_QUALITY_BOOST_ENABLE, ParamEncoderStatic, L"High motion quality boost mode enabled(integer, default = 0)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_LOWLATENCY_MODE, ParamEncoderStatic, L"Enables low latency mode (true, false default =  false)", ParamConverterBoolean);

    // Picture control properties
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_DE_BLOCKING_FILTER_DISABLE, ParamEncoderStatic, L"De-blocking Filter(true, false default =  false)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_NUM_GOPS_PER_IDR, ParamEncoderStatic, L"The frequency to insert IDR as start of a GOP. 0 means no IDR will be inserted (in frames, default= 60 )", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_GOP_SIZE, ParamEncoderStatic, L"GOP Size (in frames, default= 60 )", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_HEADER_INSERTION_MODE, ParamEncoderStatic, L"insertion mode (none, gop, idr default = none", ParamConverterInsertionModeHEVC);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_SLICES_PER_FRAME, ParamEncoderStatic, L"Slices Per Frame (integer, default = 1)", ParamConverterInt64);


// Rate control properties
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_FRAMERATE, ParamEncoderStatic, L"Frame Rate (num,den), default = depends on USAGE)", ParamConverterRate);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_VBV_BUFFER_SIZE, ParamEncoderStatic, L"VBV Buffer Size (in bits, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_INITIAL_VBV_BUFFER_FULLNESS, ParamEncoderStatic, L"Initial VBV Buffer Fullness (integer, 0=0% 64=100% , default = 64)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_TARGET_BITRATE, ParamEncoderDynamic, L"Target bit rate (in bits, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_PEAK_BITRATE, ParamEncoderDynamic, L"Peak bit rate (in bits, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_METHOD, ParamEncoderStatic, L"Rate Control Method (CQP, CBR, VBR, VBR_LAT default = depends on USAGE)", ParamConverterRateControlHEVC);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_PREENCODE_ENABLE, ParamEncoderStatic, L"Enable 2 Pass Preanalysis(true, false default = depends on USAGE)", ParamConverterBoolean);

    // Motion estimation
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MOTION_HALF_PIXEL, ParamEncoderStatic, L"Half Pixel (true, false default =  true)" , ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MOTION_QUARTERPIXEL, ParamEncoderStatic, L"Quarter Pixel (true, false default =  true" , ParamConverterBoolean);

    // ------------- Encoder params dynamic ---------------
//    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_WIDTH, ParamEncoderDynamic, L"Frame width (integer, default = 0)");
//    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_HEIGHT, ParamEncoderDynamic, L"Frame height (integer, default = 0)");


// Rate control properties
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_ENFORCE_HRD, ParamEncoderDynamic, L"Enforce HRD (true, false default = depends on USAGE)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_FILLER_DATA_ENABLE, ParamEncoderDynamic, L"Filler Data Enable (true, false default =  false)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MAX_AU_SIZE, ParamEncoderDynamic, L"Max AU Size (in bits, default = 0)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MIN_QP_I, ParamEncoderDynamic, L"Min QP I frame (integer 0-51, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MAX_QP_I, ParamEncoderDynamic, L"Max QP I frame (integer 0-51, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MIN_QP_P, ParamEncoderDynamic, L"Min QP I frame (integer 0-51, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MAX_QP_P, ParamEncoderDynamic, L"Max QP I frame (integer 0-51, default = depends on USAGE)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_QP_I, ParamEncoderDynamic, L"QP I (integer 0-51, default = 22)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_QP_P, ParamEncoderDynamic, L"QP P (integer 0-51, default = 22)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_RATE_CONTROL_SKIP_FRAME_ENABLE, ParamEncoderDynamic, L"Rate Control Based Frame Skip (true, false default =  depends on USAGE)", ParamConverterBoolean);
    
    // ------------- Encoder params per frame ---------------
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_INSERT_HEADER, ParamEncoderFrame, L"Insert Header (true, false default =  false)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_INSERT_AUD, ParamEncoderFrame, L"Insert AUD (true, false default =  false)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_MARK_CURRENT_WITH_LTR_INDEX, ParamEncoderFrame, L"Mark With LTR Index (integer, default -1)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_FORCE_LTR_REFERENCE_BITFIELD, ParamEncoderFrame, L"Force LTR Reference Bitfield (bitfield default = 0)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_FORCE_PICTURE_TYPE, ParamEncoderFrame, L"Force Picture Type (NONE, SKIP, IDR, I, P, B, default = NONE)", ParamConverterPictureTypeHEVC);

    // PA parameters
    pParams->SetParamDescription(AMF_VIDEO_ENCODER_HEVC_PRE_ANALYSIS_ENABLE, ParamEncoderStatic, L"Enable PA (true, false default =  false)", ParamConverterBoolean);

    pParams->SetParamDescription(AMF_PA_ENGINE_TYPE, ParamEncoderStatic, L"Engine Type (DX11, OPENCL, HOST default = OPENCL)", ParamConverterMemoryType);

    pParams->SetParamDescription(AMF_PA_SCENE_CHANGE_DETECTION_ENABLE, ParamEncoderDynamic, L"Scene Change Detection Enable (true, false default =  true)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_PA_SCENE_CHANGE_DETECTION_SENSITIVITY, ParamEncoderDynamic, L"Scene Change Detection Sensitivity (LOW, MEDIUM, HIGH default = MEDIUM)", ParamConverterSceneChangeHEVC);
    pParams->SetParamDescription(AMF_PA_STATIC_SCENE_DETECTION_ENABLE, ParamEncoderDynamic, L"Static Scene Detection Enable (true, false default =  true)", ParamConverterBoolean);
    pParams->SetParamDescription(AMF_PA_STATIC_SCENE_DETECTION_SENSITIVITY, ParamEncoderDynamic, L"Scene Change Detection Sensitivity (LOW, MEDIUM, HIGH default = HIGH)", ParamConverterStaticSceneHEVC);
    pParams->SetParamDescription(AMF_PA_ACTIVITY_TYPE, ParamEncoderDynamic, L"Activity Type (Y, YUV default = Y)", ParamConverterActivityTypeHEVC);

    pParams->SetParamDescription(AMF_PA_INITIAL_QP_AFTER_SCENE_CHANGE, ParamEncoderDynamic, L"QP After Scene Change (integer 0-51, default = 0)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_PA_MAX_QP_BEFORE_FORCE_SKIP, ParamEncoderDynamic, L"Max QP Before Force Skip (integer 0-51, default = 35)", ParamConverterInt64);
//  pParams->SetParamDescription(AMF_IN_FRAME_QP_FILTERING_STRENGTH, ParamEncoderDynamic, L"In Frame QP Filtering Strength (integer 0-255, default = 0)", ParamConverterInt64);
//  pParams->SetParamDescription(AMF_BETWEEN_FRAME_QP_FILTERING_STRENGTH, ParamEncoderDynamic, L"Between Frame QP Filtering Strength (integer 0-255, default = 0)", ParamConverterInt64);
    pParams->SetParamDescription(AMF_PA_CAQ_STRENGTH, ParamEncoderDynamic, L"CAQ Strength (LOW, MEDIUM, HIGH default = MEDIUM)", ParamConverterCAQStrengthHEVC);
    
    return AMF_OK;
}
