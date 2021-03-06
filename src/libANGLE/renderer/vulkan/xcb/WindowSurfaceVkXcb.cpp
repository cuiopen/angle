//
// Copyright 2016 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// WindowSurfaceVkXcb.cpp:
//    Implements the class methods for WindowSurfaceVkXcb.
//

#include "libANGLE/renderer/vulkan/xcb/WindowSurfaceVkXcb.h"

#include "libANGLE/renderer/vulkan/RendererVk.h"

namespace rx
{

WindowSurfaceVkXcb::WindowSurfaceVkXcb(const egl::SurfaceState &surfaceState,
                                       EGLNativeWindowType window,
                                       EGLint width,
                                       EGLint height,
                                       xcb_connection_t *conn)
    : WindowSurfaceVk(surfaceState, window, width, height), mXcbConnection(conn)
{
}

vk::ErrorOrResult<gl::Extents> WindowSurfaceVkXcb::createSurfaceVk(RendererVk *renderer)
{
    VkXcbSurfaceCreateInfoKHR createInfo;

    createInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    createInfo.pNext      = nullptr;
    createInfo.flags      = 0;
    createInfo.connection = mXcbConnection;
    createInfo.window     = mNativeWindowType;
    ANGLE_VK_TRY(vkCreateXcbSurfaceKHR(renderer->getInstance(), &createInfo, nullptr, &mSurface));

    xcb_get_geometry_cookie_t cookie = xcb_get_geometry(mXcbConnection, mNativeWindowType);
    xcb_get_geometry_reply_t *reply  = xcb_get_geometry_reply(mXcbConnection, cookie, nullptr);
    ASSERT(reply);
    gl::Extents result(reply->width, reply->height, 0);
    free(reply);
    return result;
}

}  // namespace rx
