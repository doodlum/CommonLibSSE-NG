#pragma once

#include "RE/B/BSTArray.h"
#include "RE/N/NiCamera.h"
#include "RE/N/NiSmartPointer.h"
#include "RE/N/NiSourceTexture.h"
#include "REL/RuntimeDataAccessors.h"
#include "SKSE/Version.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;

namespace RE
{
	class NiCamera;

	namespace BSGraphics
	{
		//WARNING: Structs containing ViewData appear to break when returned via RelocateMember due to incorrect offsets.
		struct alignas(16) ViewData
		{
			Vector4  viewUp;                            // 00
			Vector4  viewRight;                         // 10
			Vector4  viewForward;                       // 20
			Matrix   viewMat;                           // 30
			Matrix   projMat;                           // 70
			Matrix   viewProjMat;                       // B0
			Matrix   unknownMat1;                       // F0 - all 0?
			Matrix   viewProjMatrixUnjittered;          // 130
			Matrix   previousViewProjMatrixUnjittered;  // 170
			Matrix   projMatrixUnjittered;              // 1B0
			Matrix   unknownMat2;                       // 1F0 - all 0?
			float    viewPort[4];                       // 230 - NiRect<float> { left = 0, right = 1, top = 1, bottom = 0 }
			NiPoint2 viewDepthRange;                    // 240
			char     _pad0[0x8];                        // 248
		};
		static_assert(sizeof(ViewData) == 0x250);
		static_assert(offsetof(ViewData, viewUp) == 0);
		static_assert(offsetof(ViewData, viewRight) == 0x10);
		static_assert(offsetof(ViewData, viewForward) == 0x20);
		static_assert(offsetof(ViewData, viewMat) == 0x30);
		static_assert(offsetof(ViewData, projMat) == 0x70);
		static_assert(offsetof(ViewData, viewProjMat) == 0xb0);
		static_assert(offsetof(ViewData, unknownMat1) == 0xf0);
		static_assert(offsetof(ViewData, viewProjMatrixUnjittered) == 0x130);
		static_assert(offsetof(ViewData, previousViewProjMatrixUnjittered) == 0x170);
		static_assert(offsetof(ViewData, projMatrixUnjittered) == 0x1b0);
		static_assert(offsetof(ViewData, unknownMat2) == 0x1f0);
		static_assert(offsetof(ViewData, viewPort) == 0x230);
		static_assert(offsetof(ViewData, viewDepthRange) == 0x240);

		// NOTE: alignas(16) on ViewData causes padding when embedded in structs.
		// This affects FLATRIM builds where ViewData is embedded directly.
#pragma warning(push)
#pragma warning(disable: 4324)  // structure was padded due to alignment specifier
		struct CAMERASTATE_RUNTIME_DATA
		{
#if !defined(ENABLE_SKYRIM_VR)  // Non-VR
#	define CAMERASTATE_RUNTIME_DATA_CONTENT                                                               \
		ViewData camViewData;       /* 08 VR is BSTArray, Each array has 2 elements (one for each eye?) */ \
		NiPoint3 posAdjust;         /* 20 */                                                               \
		NiPoint3 currentPosAdjust;  /* 38 */                                                               \
		NiPoint3 previousPosAdjust; /* 50 */
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
#	define CAMERASTATE_RUNTIME_DATA_CONTENT                                                                         \
		BSTArray<ViewData> camViewData;       /* 08 VR is BSTArray, Each array has 2 elements (one for each eye?) */ \
		BSTArray<NiPoint3> posAdjust;         /* 20 */                                                               \
		BSTArray<NiPoint3> currentPosAdjust;  /* 38 */                                                               \
		BSTArray<NiPoint3> previousPosAdjust; /* 50 */
#else
#	define CAMERASTATE_RUNTIME_DATA_CONTENT
#endif
			//members
			CAMERASTATE_RUNTIME_DATA_CONTENT;  // 08
			bool     useJitter;                /* 68 */
			uint32_t numData;                  /* 6c */
		};

		class CameraStateData
		{
		public:
			[[nodiscard]] CAMERASTATE_RUNTIME_DATA& GetCameraStateRuntimeData() noexcept
			{
				return REL::RelocateMember<CAMERASTATE_RUNTIME_DATA>(this, 0x8, 0x8);
			}

			[[nodiscard]] inline const CAMERASTATE_RUNTIME_DATA& GetCameraStateRuntimeData() const noexcept
			{
				return REL::RelocateMember<CAMERASTATE_RUNTIME_DATA>(this, 0x8, 0x8);
			}

			// members
			NiCamera* referenceCamera;         /* 00 */
			CAMERASTATE_RUNTIME_DATA_CONTENT;  // 08
		};
#pragma warning(pop)
#if defined(EXCLUSIVE_SKYRIM_SE) || defined(EXCLUSIVE_SKYRIM_AE)  // SE/AE only
		static_assert(sizeof(CameraStateData) == 0x290);
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR only
		static_assert(sizeof(CameraStateData) == 0x68);
#elif defined(SKYRIM_CROSS_VR)      // Multi-runtime (ALL)
		static_assert(sizeof(CameraStateData) == 0x8);
#endif
		// FLATRIM: Size check skipped - alignas(16) padding causes indeterminate size
		class State
		{
		public:
			struct AE1799_RUNTIME_DATA
			{
				float         unk4C;       // 00
				float         unk50;       // 04
				std::uint32_t frameCount;  // 08
				std::uint32_t unk58;       // 0C
			};
			static_assert(sizeof(AE1799_RUNTIME_DATA) == 0x10);
			static_assert(offsetof(AE1799_RUNTIME_DATA, frameCount) == 0x8);
			static_assert(offsetof(AE1799_RUNTIME_DATA, unk58) == 0xC);
			static_assert(0x4C + offsetof(AE1799_RUNTIME_DATA, frameCount) == 0x54);
			static_assert(0x4C + offsetof(AE1799_RUNTIME_DATA, unk58) == 0x58);

			struct RUNTIME_DATA
			{
#if defined(EXCLUSIVE_SKYRIM_VR)  // VR
#	define RUNTIME_DATA_CONTENT                                                                                               \
		uint32_t                   eyeIndex;                             /* 058 -- current render eye (0 = left, 1 = right) */ \
		NiPointer<NiSourceTexture> defaultTextureBlack;                  /* 060 - "BSShader_DefHeightMap" */                   \
		NiPointer<NiSourceTexture> defaultTextureWhite;                  /* 068 */                                             \
		NiPointer<NiSourceTexture> defaultTextureGrey;                   /* 070 */                                             \
		NiPointer<NiSourceTexture> defaultHeightMap;                     /* 078 */                                             \
		NiPointer<NiSourceTexture> defaultReflectionCubeMap;             /* 080 */                                             \
		NiPointer<NiSourceTexture> defaultFaceDetailMap;                 /* 088 */                                             \
		NiPointer<NiSourceTexture> defaultTexEffectMap;                  /* 090 */                                             \
		NiPointer<NiSourceTexture> defaultTextureNormalMap;              /* 098 */                                             \
		NiPointer<NiSourceTexture> defaultTextureDitherNoiseMap;         /* 0A0 */                                             \
		BSTArray<CameraStateData>  cameraDataCacheA;                     /* 0A8 */                                             \
		std::uint32_t              unk0C0;                               /* 0C0 */                                             \
		float                      haltonSequence[2][8];                 /* 0C4 - (2, 3) Halton sequence points */             \
		float                      dynamicResolutionWidthRatio;          /* 104 */                                             \
		float                      dynamicResolutionHeightRatio;         /* 108 */                                             \
		float                      dynamicResolutionPreviousWidthRatio;  /* 10C */                                             \
		float                      dynamicResolutionPreviousHeightRatio; /* 110 */                                             \
		std::uint32_t              dynamicResolutionIncreaseFrameWaited; /* 114 */                                             \
		volatile std::int32_t      dynamicResolutionLock;                /* 118 */                                             \
		bool                       canIncreaseDynamicResolution;         /* 11C */                                             \
		bool                       canDecreaseDynamicResolution;         /* 11D */                                             \
		bool                       canChangeDynamicResolution;           /* 11E */

#else
				// Absolute offsets: SE / AE 1.6 / AE 1.7 / VR.
#	define RUNTIME_DATA_CONTENT                                                                                               \
		NiPointer<NiSourceTexture> defaultTextureBlack;                  /* 058 / 060 / 070 / 060 - "BSShader_DefHeightMap" */ \
		NiPointer<NiSourceTexture> defaultTextureWhite;                  /* 060 / 068 / 078 / 068 */                           \
		NiPointer<NiSourceTexture> defaultTextureGrey;                   /* 068 / 070 / 080 / 070 */                           \
		NiPointer<NiSourceTexture> defaultHeightMap;                     /* 070 / 078 / 088 / 078 */                           \
		NiPointer<NiSourceTexture> defaultReflectionCubeMap;             /* 078 / 080 / 090 / 080 */                           \
		NiPointer<NiSourceTexture> defaultFaceDetailMap;                 /* 080 / 088 / 098 / 088 */                           \
		NiPointer<NiSourceTexture> defaultTexEffectMap;                  /* 088 / 090 / 0A0 / 090 */                           \
		NiPointer<NiSourceTexture> defaultTextureNormalMap;              /* 090 / 098 / 0A8 / 098 */                           \
		NiPointer<NiSourceTexture> defaultTextureDitherNoiseMap;         /* 098 / 0A0 / 0B0 / 0A0 */                           \
		BSTArray<CameraStateData>  cameraDataCacheA;                     /* 0A0 / 0A8 / 0B8 / 0A8 */                           \
		std::uint32_t              unk0C0;                               /* 0B8 / 0C0 / 0D0 / 0C0 */                           \
		float                      haltonSequence[2][8];                 /* 0BC / 0C4 / 0D4 / 0C4 - (2, 3) Halton points */    \
		float                      dynamicResolutionWidthRatio;          /* 0FC / 104 / 114 / 104 */                           \
		float                      dynamicResolutionHeightRatio;         /* 100 / 108 / 118 / 108 */                           \
		float                      dynamicResolutionPreviousWidthRatio;  /* 104 / 10C / 11C / 10C */                           \
		float                      dynamicResolutionPreviousHeightRatio; /* 108 / 110 / 120 / 110 */                           \
		std::uint32_t              dynamicResolutionIncreaseFrameWaited; /* 10C / 114 / 124 / 114 */                           \
		volatile std::int32_t      dynamicResolutionLock;                /* 110 / 118 / 128 / 118 */                           \
		bool                       canIncreaseDynamicResolution;         /* 114 / 11C / 12C / 11C */                           \
		bool                       canDecreaseDynamicResolution;         /* 115 / 11D / 12D / 11D */                           \
		bool                       canChangeDynamicResolution;           /* 116 / 11E / 12E / 11E */
#endif
				RUNTIME_DATA_CONTENT;
			};
#if !defined(ENABLE_SKYRIM_VR)  // Non-VR
			static_assert(offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0xA4);
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
			static_assert(offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0xAC);
#else
			static_assert(offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0xA4);
#endif
			[[nodiscard]] static State* GetSingleton()
			{
				static REL::Relocation<State*> singleton{ RELOCATION_ID(524998, 411479) };
				return singleton.get();
			}

			CameraStateData* FindCameraDataCache(const NiCamera* a_camera, bool a_useJitter)
			{
				for (uint32_t i = 0; i < GetRuntimeData().cameraDataCacheA.size(); i++) {
					if (GetRuntimeData().cameraDataCacheA[i].referenceCamera == a_camera &&
						GetRuntimeData().cameraDataCacheA[i].GetCameraStateRuntimeData().useJitter == a_useJitter)
						return &GetRuntimeData().cameraDataCacheA[i];
				}
				return nullptr;
			}

			RUNTIME_MEMBER_ACCESSOR_VERSIONED(std::uint32_t, GetFrameCount, SKSE::RUNTIME_SSE_1_7_99, 0x4C, 0x4C, 0x54);

			[[nodiscard]] AE1799_RUNTIME_DATA* GetAe1799RuntimeData() noexcept
			{
				if SKYRIM_REL_CONSTEXPR (!REL::Module::IsAE()) {
					return nullptr;
				}
				if (!REL::Module::IsAtLeast(SKSE::RUNTIME_SSE_1_7_99)) {
					return nullptr;
				}
				return &REL::RelocateMember<AE1799_RUNTIME_DATA>(this, 0x4C);
			}

			[[nodiscard]] const AE1799_RUNTIME_DATA* GetAe1799RuntimeData() const noexcept
			{
				return const_cast<State*>(this)->GetAe1799RuntimeData();
			}

#if defined(EXCLUSIVE_SKYRIM_VR)
			// The exclusive-VR RUNTIME_DATA includes firstCameraStateIndex at State + 0x58.
			// Cross-runtime RUNTIME_DATA omits that field and therefore begins at State + 0x60.
			static constexpr std::size_t RUNTIME_DATA_VR_OFFSET = 0x58;
#else
			static constexpr std::size_t RUNTIME_DATA_VR_OFFSET = 0x60;
#endif
			[[nodiscard]] RUNTIME_DATA& GetRuntimeData() noexcept
			{
				if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
					if (REL::Module::IsAtLeast(SKSE::RUNTIME_SSE_1_7_99)) {
						return REL::RelocateMember<RUNTIME_DATA>(this, 0x70);
					}
					return REL::RelocateMemberIfNewer<RUNTIME_DATA>(SKSE::RUNTIME_SSE_1_6_629, this, 0x58, 0x60);
				}
				return REL::RelocateMember<RUNTIME_DATA>(this, 0x58, RUNTIME_DATA_VR_OFFSET);
			}

			[[nodiscard]] const RUNTIME_DATA& GetRuntimeData() const noexcept
			{
				return const_cast<State*>(this)->GetRuntimeData();
			}

#if defined(ENABLE_SKYRIM_VR)
			static_assert(RUNTIME_DATA_VR_OFFSET + offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0x104);
			static_assert(RUNTIME_DATA_VR_OFFSET + offsetof(RUNTIME_DATA, dynamicResolutionLock) == 0x118);
#endif
#if defined(ENABLE_SKYRIM_AE)
			static_assert(0x60 + offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0x104);
			static_assert(0x60 + offsetof(RUNTIME_DATA, dynamicResolutionLock) == 0x118);
			static_assert(0x70 + offsetof(RUNTIME_DATA, dynamicResolutionWidthRatio) == 0x114);
			static_assert(0x70 + offsetof(RUNTIME_DATA, dynamicResolutionLock) == 0x128);
#endif

			[[nodiscard]] inline bool GetDoubleDynamicResolutionAdjustmentFrequency() noexcept
			{
				if SKYRIM_REL_CONSTEXPR (REL::Module::IsAE()) {
					// The 1.7.99 prefix shifts the established AE field by 0xC.
					return REL::RelocateMemberIfNewer<bool>(SKSE::RUNTIME_SSE_1_7_99, this, 0x57, 0x63);
				}
				return false;
			}

			void SetCameraData(const NiCamera* a_camera, std::uint32_t a_flags)
			{
				using func_t = decltype(&State::SetCameraData);
				static REL::Relocation<func_t> func{ RELOCATION_ID(75694, 77503) };
				return func(this, a_camera, a_flags);
			}

			// members
			NiPointer<NiSourceTexture> defaultTextureProjNoiseMap;         // 000
			NiPointer<NiSourceTexture> defaultTextureProjDiffuseMap;       // 008
			NiPointer<NiSourceTexture> defaultTextureProjNormalMap;        // 010
			NiPointer<NiSourceTexture> defaultTextureProjNormalDetailMap;  // 018
			std::uint32_t              unk20;                              // 020
			std::uint32_t              screenWidth;                        // 024
			std::uint32_t              screenHeight;                       // 028
			std::uint32_t              frameBufferViewport[2];             // 02C
			std::uint32_t              unk34;                              // 034
			std::uint32_t              unk38;                              // 038
			std::uint32_t              unk03C;                             // 03C
			std::uint32_t              unk040;                             // 040
			float                      projectionPosScaleX;                // 044
			float                      projectionPosScaleY;                // 048
#if defined(EXCLUSIVE_SKYRIM_SE)
			std::uint32_t frameCount;               // 04C
			bool          insideFrame;              // 050
			bool          letterbox;                // 051
			bool          unk052;                   // 052
			bool          compiledShaderThisFrame;  // 053
			bool          unk054;                   // 054
			bool          useEarlyZ;                // 055
			RUNTIME_DATA_CONTENT;                   // 058
#elif defined(EXCLUSIVE_SKYRIM_VR)
			// Retained from v6.6.0; this port did not independently re-audit the VR prefix.
			std::uint32_t frameCount;               // 04C
			bool          unk50;                    // 050
			bool          letterbox;                // 051
			bool          unk052;                   // 052
			bool          compiledShaderThisFrame;  // 053
			bool          insideFrame;              // 054
			bool          unk055;                   // 055
			RUNTIME_DATA_CONTENT;                   // 058
#else
			std::uint8_t pad4C[0xC];  // 04C
#endif
		};
#if defined(EXCLUSIVE_SKYRIM_SE)  // SE
		static_assert(sizeof(State) == 0x118);
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
		static_assert(offsetof(State, letterbox) == 0x51);
		static_assert(offsetof(State, insideFrame) == 0x50);
		static_assert(offsetof(State, useEarlyZ) == 0x55);
		static_assert(offsetof(State, defaultTextureBlack) == 0x58);
		static_assert(offsetof(State, defaultTextureWhite) == 0x60);
		static_assert(offsetof(State, cameraDataCacheA) == 0xa0);
		static_assert(offsetof(State, dynamicResolutionWidthRatio) == 0x0fc);
#elif defined(EXCLUSIVE_SKYRIM_VR)  // VR
		static_assert(sizeof(State) == 0x120);
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
		static_assert(offsetof(State, letterbox) == 0x51);
		static_assert(offsetof(State, insideFrame) == 0x54);
		static_assert(offsetof(State, defaultTextureBlack) == 0x60);
		static_assert(offsetof(State, defaultTextureWhite) == 0x68);
		static_assert(offsetof(State, cameraDataCacheA) == 0xa8);
		static_assert(offsetof(State, dynamicResolutionWidthRatio) == 0x104);
#else
		static_assert(sizeof(State) == 0x58);
		static_assert(offsetof(State, screenWidth) == 0x24);
		static_assert(offsetof(State, frameBufferViewport) == 0x2C);
		static_assert(offsetof(State, projectionPosScaleX) == 0x44);
		static_assert(offsetof(State, projectionPosScaleY) == 0x48);
		static_assert(offsetof(State, pad4C) == 0x4C);
#endif
	}
}

#ifdef FMT_VERSION
template <>
struct fmt::formatter<Vector3>
{
	// Presentation format: 'f' - fixed, 'e' - exponential.
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator
	{
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'f' || *it == 'e'))
			presentation = *it++;

		// Check if reached the end of the range:
		if (it != end && *it != '}')
			format_error("invalid format");

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format(const Vector3& v, format_context& ctx) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f' ? fmt::format_to(ctx.out(), "[{:.1f}, {:.1f}, {:.1f}]", v.x, v.y, v.z) : fmt::format_to(ctx.out(), "[{:.1e}, {:.1e}, {:.1e}]", v.x, v.y, v.z);
	}
};

template <>
struct fmt::formatter<Vector4>
{
	// Presentation format: 'f' - fixed, 'e' - exponential.
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator
	{
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'f' || *it == 'e'))
			presentation = *it++;

		// Check if reached the end of the range:
		if (it != end && *it != '}')
			format_error("invalid format");

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format(const Vector4& v, format_context& ctx) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return presentation == 'f' ? fmt::format_to(ctx.out(), "[{:.1f}, {:.1f}, {:.1f}, {:.1f}]", v.x, v.y, v.z, v.w) : fmt::format_to(ctx.out(), "[{:.1e}, {:.1e}, {:.1e}, {:.1e}]", v.x, v.y, v.z, v.w);
	}
};

template <>
struct fmt::formatter<Matrix>
{
	char presentation = 'f';

	// Parses format specifications of the form ['f' | 'e'].
	constexpr auto parse(format_parse_context& ctx) -> format_parse_context::iterator
	{
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'f' || *it == 'e'))
			presentation = *it++;

		// Check if reached the end of the range:
		if (it != end && *it != '}')
			format_error("invalid format");

		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the point p using the parsed format specification (presentation)
	// stored in this formatter.
	auto format(const Matrix& m, format_context& ctx) const -> format_context::iterator
	{
		// ctx.out() is an output iterator to write to.
		return fmt::format_to(ctx.out(), "[{}, {}, {}, {}]", (Vector4)m.m[0], (Vector4)m.m[1], (Vector4)m.m[2], (Vector4)m.m[3]);
	}
};
#endif
#undef RUNTIME_DATA_CONTENT
