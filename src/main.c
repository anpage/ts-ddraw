/*
 * Copyright (c) 2013 Toni Spets <toni.spets@iki.fi>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "main.h"
#include "IDirectDraw.h"

int PROXY = 0;
int VERBOSE = 1;
static HANDLE real_dll = NULL;
static HRESULT WINAPI (*real_DirectDrawCreate)(GUID FAR*, LPDIRECTDRAW FAR*, IUnknown FAR*) = NULL;

HRESULT WINAPI DirectDrawCreate(GUID FAR* lpGUID, LPDIRECTDRAW FAR* lplpDD, IUnknown FAR* pUnkOuter) 
{
    dprintf("DirectDrawCreate(lpGUID=%p, lplpDD=%p, pUnkOuter=%p)\n", lpGUID, lplpDD, pUnkOuter);

    char buf[32];
    buf[0] = '\0';
    GetEnvironmentVariable("DDRAW_PROXY", buf, sizeof buf);
    if (buf[0]) PROXY = 1;
    buf[0] = '\0';
    GetEnvironmentVariable("DDRAW_LESS", buf, sizeof buf);
    if (buf[0]) VERBOSE = 0;

    IDirectDrawImpl *ddraw = IDirectDrawImpl_construct();

    if (PROXY)
    {
        real_dll = LoadLibrary("system32\\ddraw.dll");
        dprintf(" real_dll = %p\n", real_dll);
        real_DirectDrawCreate = (HRESULT WINAPI (*)(GUID FAR*, LPDIRECTDRAW FAR*, IUnknown FAR*))GetProcAddress(real_dll, "DirectDrawCreate");
        dprintf(" real_DirectDrawCreate = %p\n", real_DirectDrawCreate);
        real_DirectDrawCreate(lpGUID, &ddraw->real, pUnkOuter);
    }

    *lplpDD = (IDirectDraw *)ddraw;
    dprintf(" lplpDD = %p\n", *lplpDD);

    return DD_OK;
}

void dump_dwcaps(DWORD dwCaps)
{
    if (!VERBOSE) return;
    dprintf("-- DWCAPS --\n");
    if (dwCaps & DDCAPS_3D)
        dprintf("    DDCAPS_3D\n");

    if (dwCaps & DDCAPS_BLT)
        dprintf("    DDCAPS_BLT\n");

    dprintf("    Warning: Not all dwCaps printed!\n");
    dprintf("------------\n");
}

void dump_ddcaps(LPDDCAPS lpDDCaps)
{
    if (!VERBOSE) return;
    dprintf("-- DDCAPS --\n");
    dprintf("    dwSize     = %d\n", (int)lpDDCaps->dwSize);
    dprintf("    dwCaps     = %08X\n", (int)lpDDCaps->dwCaps);
    dprintf("    dwCaps2    = %08X\n", (int)lpDDCaps->dwCaps2);
    dprintf("------------\n");
    dump_dwcaps(lpDDCaps->dwCaps);
}

void dump_ddscaps(LPDDSCAPS lpDDSCaps)
{
    if (!VERBOSE) return;
    dprintf("-- DDSCAPS --\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_ALPHA)
        dprintf("    DDSCAPS_ALPHA\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_BACKBUFFER)
        dprintf("    DDSCAPS_BACKBUFFER\n");

#if 0
    if (lpDDSCaps->dwCaps & DDSCAPS_DYNAMIC)
        dprintf("    DDSCAPS_DYNAMIC\n");
#endif

    if (lpDDSCaps->dwCaps & DDSCAPS_FLIP)
        dprintf("    DDSCAPS_FLIP\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_FRONTBUFFER)
        dprintf("    DDSCAPS_FRONTBUFFER\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_OVERLAY)
        dprintf("    DDSCAPS_OVERLAY\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_PALETTE)
        dprintf("    DDSCAPS_PALETTE\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_PRIMARYSURFACE)
        dprintf("    DDSCAPS_PRIMARYSURFACE\n");

#if 0
    if (lpDDSCaps->dwCaps & DDSCAPS_READONLY)
        dprintf("    DDSCAPS_READONLY\n");
#endif

    if (lpDDSCaps->dwCaps & DDSCAPS_SYSTEMMEMORY)
        dprintf("    DDSCAPS_SYSTEMMEMORY\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_VIDEOMEMORY)
        dprintf("    DDSCAPS_VIDEOMEMORY\n");

    if (lpDDSCaps->dwCaps & DDSCAPS_WRITEONLY)
        dprintf("    DDSCAPS_WRITEONLY\n");

    dprintf("-------------\n");
}

void dump_ddsurfacedesc(LPDDSURFACEDESC lpDDSurfaceDesc)
{
    if (!VERBOSE) return;
    dprintf("        dwSize             = %d\n", (int)lpDDSurfaceDesc->dwSize);
    dprintf("        dwFlags            = %08X\n", (int)lpDDSurfaceDesc->dwFlags);

    if (lpDDSurfaceDesc->dwFlags & DDSD_BACKBUFFERCOUNT)
        dprintf("            DDSD_BACKBUFFERCOUNT\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
        dprintf("            DDSD_CAPS\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_CKDESTBLT)
        dprintf("            DDSD_CKDESTBLT\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_CKDESTOVERLAY)
        dprintf("            DDSD_CKDESTOVERLAY\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_CKSRCBLT)
        dprintf("            DDSD_CKSRCBLT\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_CKSRCOVERLAY)
        dprintf("            DDSD_CKSRCOVERLAY\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
        dprintf("            DDSD_HEIGHT\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_LPSURFACE)
        dprintf("            DDSD_LPSURFACE\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_PITCH)
        dprintf("            DDSD_PITCH\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_PIXELFORMAT)
        dprintf("            DDSD_PIXELFORMAT\n");

    if (lpDDSurfaceDesc->dwFlags & DDSD_REFRESHRATE)
        dprintf("            DDSD_REFRESHRATE\n");

#if 0
    if (lpDDSurfaceDesc->dwFlags & DDSD_SURFACESIZE)
        dprintf("            DDSD_SURFACESIZE\n");
#endif

    if (lpDDSurfaceDesc->dwFlags & DDSD_WIDTH)
        dprintf("            DDSD_WIDTH\n");

#if 0
    if (lpDDSurfaceDesc->dwFlags & DDSD_XPITCH)
        dprintf("            DDSD_XPITCH\n");
#endif

    if (lpDDSurfaceDesc->dwFlags & DDSD_WIDTH)
        dprintf("        dwWidth            = %d\n", (int)lpDDSurfaceDesc->dwWidth);

    if (lpDDSurfaceDesc->dwFlags & DDSD_HEIGHT)
        dprintf("        dwHeight           = %d\n", (int)lpDDSurfaceDesc->dwHeight);

    if (lpDDSurfaceDesc->dwFlags & DDSD_PITCH)
        dprintf("        lPitch             = %d\n", (int)lpDDSurfaceDesc->lPitch);

    if (lpDDSurfaceDesc->dwFlags & DDSD_BACKBUFFERCOUNT )
        dprintf("        dwBackBufferCount  = %d\n", (int)lpDDSurfaceDesc->lPitch);

    if (lpDDSurfaceDesc->dwFlags & DDSD_REFRESHRATE)
        dprintf("        dwRefreshRate      = %d\n", (int)lpDDSurfaceDesc->dwRefreshRate);

    if (lpDDSurfaceDesc->dwFlags & DDSD_LPSURFACE)
        dprintf("        lpSurface          = %p\n", lpDDSurfaceDesc->lpSurface);

    if (lpDDSurfaceDesc->dwFlags & DDSD_CAPS)
        dump_ddscaps(&lpDDSurfaceDesc->ddsCaps);
}