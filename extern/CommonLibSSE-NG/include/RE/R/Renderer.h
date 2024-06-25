#pragma once

#include "RE/B/BSShaderRenderTargets.h"
#include "RE/N/NiTexture.h"
#include "RE/R/RenderTargetData.h"
#include "RE/T/TextureFileFormat.h"
#include <SKSE/Version.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView1;
struct ID3D11ShaderResourceView1;

namespace RE
{
	namespace BSGraphics
	{
		struct RendererWindow
		{
		public:
			// members
			SKSE::WinAPI::HWND         hWnd;                   // 10
			std::int32_t               windowX;                // 18
			std::int32_t               windowY;                // 1C
			std::int32_t               windowWidth;            // 20
			std::int32_t               windowHeight;           // 24
			IDXGISwapChain*            swapChain;              // 28
			RENDER_TARGET              swapChainRenderTarget;  // 30
			std::uint32_t              unk24;                  // 34
			std::uint64_t              unk28;                  // 38
			ID3D11RenderTargetView1*   renderView;             // 40
			ID3D11ShaderResourceView1* resourceView;           // 48
			std::uint64_t              unk40;                  // 50
			std::uint64_t              unk48;                  // 58
		};
		static_assert(sizeof(RendererWindow) == 0x50);

		struct RendererData2
		{
		public:
#define RENDERER_DATA2_CONTENT                                                                              \
	CubemapRenderTargetData        cubemapRenderTargets[RENDER_TARGET_CUBEMAP::kTOTAL]; /* 26D8, VR 2E48, AE1130 2738 */ \
	Texture3DTargetData            texture3DRenderTargets[RENDER_TARGET_3D::kTOTAL];    /* 2718, VR 2E88, AE1130 2778*/  \
	float                          clearColor[4];                                       /* 2778, VR 2EE8, AE1130 27d8*/  \
	std::uint8_t                   clearStencil;                                        /* 2788, VR 2EF8, AE1130 27e8*/  \
	SKSE::WinAPI::CRITICAL_SECTION lock;                                                /* 2790, VR 2F00, AE1130 27f0*/  \
	const char*                    className;                                           /* 27B8, VR 2F28, AE1130 2818*/  \
	SKSE::WinAPI::HINSTANCE        hInstance;                                           /* 27C0, VR 2F30, AE1130 2820*/
            RENDERER_DATA2_CONTENT
		};

		struct DepthStencilRuntimeData
		{
		public:
#if !defined(ENABLE_SKYRIM_VR)
#	define DEPTHSTENCIL_RUNTIME_DATA_CONTENT \
		DepthStencilData depthStencils[RENDER_TARGET_DEPTHSTENCIL::kTOTAL]; /* 1FB8, VR 21D0, AE1130 0x2018*/
//#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
#else
#	define DEPTHSTENCIL_RUNTIME_DATA_CONTENT \
		DepthStencilData depthStencils[RENDER_TARGET_DEPTHSTENCIL::kVRTOTAL]; /* 1FB8, VR 21D0, AE1130 0x2018*/
#endif
			DEPTHSTENCIL_RUNTIME_DATA_CONTENT
		};

		struct RendererData
		{
		public:
			// members
#if !defined(ENABLE_SKYRIM_VR)
#	define RUNTIME_DATA_CONTENT                                                                            \
		std::uint32_t                     uiAdapter;                              /* 0018 */                \
		DirectX::DXGI_RATIONAL            desiredRefreshRate;                     /* 001C - refreshRate? */ \
		DirectX::DXGI_RATIONAL            actualRefreshRate;                      /* 0020 */                \
		DirectX::DXGI_MODE_SCALING        scaleMode;                              /* 0024*/                 \
		DirectX::DXGI_MODE_SCANLINE_ORDER scanlineOrdering;                       /* 0028*/                 \
		std::uint32_t                     isNotWindowed;                          /* 0034*/                 \
		bool                              fullScreen;                             /* 0038*/                 \
		bool                              borderlessDisplay;                      /* 0039*/                 \
		bool                              readOnlyDepth;                          /* 003A*/                 \
		bool                              instantiated;                           /* 003B*/                 \
		bool                              requestedWindowSizeChange;              /* 003C*/                 \
		bool                              unk25;                                  /* 003D*/                 \
		std::uint32_t                     newWidth;                               /* 0038*/                 \
		std::uint32_t                     newHeight;                              /* 0040*/                 \
		std::uint32_t                     presentInterval;                        /* 004C*/                 \
		ID3D11Device*                     forwarder;                              /* 0050*/                 \
		ID3D11DeviceContext*              context;                                /* 0058*/                 \
		RendererWindow                    renderWindows[32];                      /* 0060*/                 \
		RenderTargetData                  renderTargets[RENDER_TARGET::kVRTOTAL]; /* 0A60*/
#else
#	define RUNTIME_DATA_CONTENT                                                                          \
		std::uint32_t                     uiAdapter;                            /* 0010 */                \
		DirectX::DXGI_RATIONAL            desiredRefreshRate;                   /* 001C - refreshRate? */ \
		DirectX::DXGI_RATIONAL            actualRefreshRate;                    /* 0020 */                \
		DirectX::DXGI_MODE_SCALING        scaleMode;                            /* 0024*/                 \
		DirectX::DXGI_MODE_SCANLINE_ORDER scanlineOrdering;                     /* 0028*/                 \
		std::uint32_t                     isNotWindowed;                        /* 0034*/                 \
		bool                              fullScreen;                           /* 0038*/                 \
		bool                              borderlessDisplay;                    /* 0039*/                 \
		bool                              readOnlyDepth;                        /* 003A*/                 \
		bool                              instantiated;                         /* 003B*/                 \
		bool                              requestedWindowSizeChange;            /* 003C*/                 \
		bool                              unk25;                                /* 003D*/                 \
		std::uint32_t                     newWidth;                             /* 0038*/                 \
		std::uint32_t                     newHeight;                            /* 0040*/                 \
		std::uint32_t                     presentInterval;                      /* 004C*/                 \
		ID3D11Device*                     forwarder;                            /* 0050*/                 \
		ID3D11DeviceContext*              context;                              /* 0058*/                 \
		RendererWindow                    renderWindows[32];                    /* 0058*/                 \
		RenderTargetData                  renderTargets[RENDER_TARGET::kTOTAL]; /* 0A58*/
#endif
			RUNTIME_DATA_CONTENT;
		};
#if !defined(ENABLE_SKYRIM_VR)
		static_assert(sizeof(RendererData) == 0x21B8);
		static_assert(offsetof(RendererData, context) == 0x40);
		static_assert(offsetof(RendererData, renderTargets) == 0xa48);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
		static_assert(sizeof(RendererData) == 0x1fa8);
		static_assert(offsetof(RendererData, context) == 0x40);
		static_assert(offsetof(RendererData, renderTargets) == 0xa48);
#else
		static_assert(sizeof(RendererData) == 0x1fa8);
		static_assert(offsetof(RendererData, context) == 0x40);
		static_assert(offsetof(RendererData, renderTargets) == 0xa48);
#endif
		struct RendererInitOSData
		{
			WinAPI::HWND      hwnd;                // 00
			WinAPI::HINSTANCE instance;            // 08
			WinAPI::WNDPROC   windowProcFunction;  // 10
			WinAPI::HICON     icon;                // 18
			const char*       className;           // 20
			uint32_t          adapter;             // 28
			uint32_t          unk2C;               // 2C
		};
		static_assert(sizeof(RendererInitOSData) == 0x30);

		struct ScreenSize
		{
			uint32_t width;   // 00
			uint32_t height;  // 04
		};
		static_assert(sizeof(ScreenSize) == 0x8);

		struct ApplicationWindowProperties
		{
			ScreenSize screenSize;        // 00
			int        windowX;           // 08
			int        windowY;           // 0C
			int        refreshRate;       // 10
			int        presentInterval;   // 14
			bool       appFullScreen;     // 18
			bool       borderlessWindow;  // 19
			bool       vsync;             // 1A
		};
		static_assert(sizeof(ApplicationWindowProperties) == 0x1C);

		class Renderer
		{
		public:
			[[nodiscard]] inline RendererData& GetRuntimeData() noexcept
			{
				return REL::RelocateMember<RendererData>(this, 0x10, 0x18);
			}

			[[nodiscard]] inline const RendererData& GetRuntimeData() const noexcept
			{
				return REL::RelocateMember<RendererData>(this, 0x10, 0x18);
			}

			[[nodiscard]] inline DepthStencilRuntimeData& GetDepthStencilData() noexcept
			{
				if (REL::Module::IsAE())
					return REL::RelocateMemberIfNewer<DepthStencilRuntimeData>(SKSE::RUNTIME_SSE_1_6_1130, this, 0x1FB8, 0x2018);

				return REL::RelocateMember<DepthStencilRuntimeData>(this, 0x1FB8, 0x21D0);
			}

			[[nodiscard]] inline const DepthStencilRuntimeData& GetDepthStencilData() const noexcept
			{
				if (REL::Module::IsAE())
					return REL::RelocateMemberIfNewer<DepthStencilRuntimeData>(SKSE::RUNTIME_SSE_1_6_1130, this, 0x1FB8, 0x2018);
				return REL::RelocateMember<DepthStencilRuntimeData>(this, 0x1FB8, 0x21D0);
			}

			[[nodiscard]] inline RendererData2& GetRendererData() noexcept
			{
				if (REL::Module::IsAE())
						return REL::RelocateMemberIfNewer<RendererData2>(SKSE::RUNTIME_SSE_1_6_1130, this, 0x26D8, 0x2738);
				return REL::RelocateMember<RendererData2>(this, 0x26D8, 0x2E48);
			}

			[[nodiscard]] inline const RendererData2& GetRendererData() const noexcept
			{
				if (REL::Module::IsAE())
					return REL::RelocateMemberIfNewer<RendererData2>(SKSE::RUNTIME_SSE_1_6_1130, this, 0x26D8, 0x2738);
				return REL::RelocateMember<RendererData2>(this, 0x26D8, 0x2E48);
			}
			[[nodiscard]] inline SKSE::WinAPI::CRITICAL_SECTION& GetLock() noexcept
			{
				if (REL::Module::IsAE())
					return REL::RelocateMemberIfNewer<SKSE::WinAPI::CRITICAL_SECTION>(SKSE::RUNTIME_SSE_1_6_1130, this, 0x2790, 0x27f0);
				return REL::RelocateMember<SKSE::WinAPI::CRITICAL_SECTION>(this, 0x2790, 0x2F00);
			}

			[[nodiscard]] inline const SKSE::WinAPI::CRITICAL_SECTION& GetLock() const noexcept
			{
				if (REL::Module::IsAE())
					return REL::RelocateMemberIfNewer<SKSE::WinAPI::CRITICAL_SECTION>(SKSE::RUNTIME_SSE_1_6_1130, this, 0x2790, 0x27f0);
				return REL::RelocateMember<SKSE::WinAPI::CRITICAL_SECTION>(this, 0x2790, 0x2F00);
			}

			[[nodiscard]] static Renderer* GetSingleton() noexcept;

			void CreateSwapChain(WinAPI::HWND* window, bool setCurrent);
			void KillWindow(std::uint32_t windowID);
			void Lock();
			void Unlock();
			void ResizeWindow(std::uint32_t windowID, std::uint32_t width, std::uint32_t height, bool fullscreen, bool borderless);
			void RequestWindowResize(std::uint32_t width, std::uint32_t height);
			void SetWindowPosition(std::uint32_t windowID, std::int32_t x, std::int32_t y);
			void SetWindowActiveState(bool show);
			void WindowSizeChanged(std::uint32_t windowID);
			void ResetWindow(std::uint32_t windowID);
			void UpdateViewPort(std::uint32_t a_unk, std::uint32_t b_unk, bool c_unk);

			[[nodiscard]] NiTexture::RendererData* CreateRenderTexture(std::uint32_t a_width, std::uint32_t a_height);
			void                                   SaveRenderTargetToFile(RENDER_TARGET a_renderTarget, const char* a_filePath, TextureFileFormat a_textureFileFormat);

			RendererData* GetRendererDataSingleton();

			[[nodiscard]] static ScreenSize      GetScreenSize();
			[[nodiscard]] static ID3D11Device*   GetDevice();
			[[nodiscard]] static RendererWindow* GetCurrentRenderWindow();

		private:
			void Begin(std::uint32_t windowID);
			void Init(RendererInitOSData* a_data, ApplicationWindowProperties* windowProps, WinAPI::HWND window);
			void End();
			void Shutdown();

		public:
			// members
			std::uint64_t unk000;      // 0000
			bool          drawStereo;  // 0008
#if !defined(ENABLE_SKYRIM_VR)
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
			std::uint64_t unk010;  // 0010
#endif
			RUNTIME_DATA_CONTENT;  // 0010, VR 18
		};
#if !defined(ENABLE_SKYRIM_VR)
		static_assert(sizeof(Renderer) == 0x21C0);
#elif !defined(ENABLE_SKYRIM_AE) && !defined(ENABLE_SKYRIM_SE)
		static_assert(sizeof(Renderer) == 0x1fc0);
#else
		static_assert(sizeof(Renderer) == 0x1fb0);
#endif
	}
}
#undef RUNTIME_DATA_CONTENT
#undef DEPTHSTENCIL_RUNTIME_DATA_CONTENT
#undef RENDERER_DATA2_CONTENT
