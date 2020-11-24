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

#pragma once

#include "VideoRender.h"
#include "public/include/core/VulkanAMF.h"
#include "../common/SwapChainVulkan.h"


class VideoRenderVulkan : public VideoRender, public SwapChainVulkan 
{
public:
    VideoRenderVulkan(amf_int width, amf_int height, bool bInterlaced, amf_int frames, amf::AMFContext* pContext);
    virtual ~VideoRenderVulkan();

    virtual AMF_RESULT              Init(amf_handle hWnd, amf_handle hDisplay, bool bFullScreen);
    virtual AMF_RESULT              Terminate();
    virtual AMF_RESULT              Render(amf::AMFData** ppData);
#if defined(_WIN32)
    virtual amf::AMF_SURFACE_FORMAT GetFormat() { return amf::AMF_SURFACE_BGRA; }
#elif defined(__linux)    
    virtual amf::AMF_SURFACE_FORMAT GetFormat() { return amf::AMF_SURFACE_BGRA; }
#endif
protected:
    AMF_RESULT CreatePipelineInput();
    AMF_RESULT CreateDescriptorSetLayout();
    AMF_RESULT CreatePipeline();
    AMF_RESULT CreateCommands();
    AMF_RESULT CreateVertexBuffers();
    AMF_RESULT CreateDescriptorSetPool();
    AMF_RESULT UpdateMVP();

    float           m_fAnimation;


    std::vector<VkCommandBuffer>	m_CommandBuffers;

    VkDescriptorSetLayout			m_hUniformLayout;
    VkPipelineLayout				m_hPipelineLayout;
    VkPipeline						m_hPipeline;


    amf::AMFVulkanBuffer            m_VertexBuffer;
    amf::AMFVulkanBuffer            m_IndexBuffer;
    amf::AMFVulkanBuffer            m_MVPBuffer;

    VkDescriptorSet					m_hDescriptorSet;
    VkDescriptorPool				m_hDescriptorPool;
};

