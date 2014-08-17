#include <thin3d/thin3d.h>

static const char * const glsl_fsTexCol =
"varying vec4 oColor0;\n"
"varying vec2 oTexCoord0;\n"
"uniform sampler2D Sampler0;\n"
"void main() { gl_FragColor = oColor0 * texture2D(Sampler0, oTexCoord0); }\n";

static const char * const hlslFsTexCol =
"struct PS_INPUT { float4 color : COLOR0; float2 uv : TEXCOORD0; };\n"
"sampler2D tex0;\n"
"float4 main(PS_INPUT input) : COLOR0 {\n"
"  return input.color * tex2D(tex0, input.uv);\n"
"}\n";

static const char * const glsl_fsCol =
"varying vec4 oColor0;\n"
"uniform sampler2D Sampler0;\n"
"void main() { gl_FragColor = oColor0; }\n";

static const char * const hlslFsCol =
"struct PS_INPUT { float4 color : COLOR0; };\n"
"float4 main(PS_INPUT input) : COLOR0 {\n"
"  return input.color;\n"
"}\n";

static const char * const glsl_vsCol =
"attribute vec3 Position;\n"
"attribute vec4 Color0;\n"
"varying vec4 oColor0;\n"
"uniform mat4 WorldViewProj;\n"
"void main() {\n"
"	gl_Position = WorldViewProj * vec4(Position, 1.0);\n"
"	oColor0 = Color0;\n"
"}";

static const char * const hlslVsCol =
"struct VS_INPUT { float3 Position : POSITION; float4 Color0 : COLOR0; };\n"
"struct VS_OUTPUT { float4 Position : POSITION; float4 Color0 : COLOR0; };\n"
"float4x4 WorldViewProj : WORLDVIEWPROJ;\n"
"VS_OUTPUT main(VS_INPUT input) {\n"
"  VS_OUTPUT output;\n"
"  output.Position = mul(float4(input.Position, 1.0), WorldViewProj);\n"
"  output.Color0 = input.Color0;\n"
"  return output;\n"
"}\n";

static const char * const glsl_vsTexCol =
"attribute vec3 Position;\n"
"attribute vec4 Color0;\n"
"attribute vec2 TexCoord0;\n"
"varying vec4 oColor0;\n"
"varying vec2 oTexCoord0;\n"
"uniform mat4 WorldViewProj;\n"
"void main() {\n"
"	gl_Position = WorldViewProj * vec4(Position, 1.0);\n"
"	oColor0 = Color0;\n"
" oTexCoord0 = TexCoord0; \n"
"}\n";

static const char * const hlslVsTexCol =
"struct VS_INPUT { float3 Position : POSITION; float2 Texcoord0 : TEXCOORD0; float4 Color0 : COLOR0; };\n"
"struct VS_OUTPUT { float4 Position : POSITION; float2 Texcoord0 : TEXCOORD0; float4 Color0 : COLOR0; };\n"
"float4x4 WorldViewProj : WORLDVIEWPROJ;\n"
"VS_OUTPUT main(VS_INPUT input) {\n"
"  VS_OUTPUT output;\n"
"  output.Position = mul(float4(input.Position, 1.0), WorldViewProj);\n"
"  output.Texcoord0 = input.Texcoord0;\n"
"  output.Color0 = input.Color0;\n"
"  return output;\n"
"}\n";

void Thin3DContext::CreatePresets() {
	// Build prebuilt objects
	T3DBlendStateDesc off = { false };
	T3DBlendStateDesc additive = { true, T3DBlendEquation::ADD, T3DBlendFactor::ONE, T3DBlendFactor::ONE, T3DBlendEquation::ADD, T3DBlendFactor::ONE, T3DBlendFactor::ZERO };
	T3DBlendStateDesc standard_alpha = { true, T3DBlendEquation::ADD, T3DBlendFactor::SRC_ALPHA, T3DBlendFactor::ONE_MINUS_SRC_ALPHA, T3DBlendEquation::ADD, T3DBlendFactor::ONE, T3DBlendFactor::ZERO };
	T3DBlendStateDesc premul_alpha = { true, T3DBlendEquation::ADD, T3DBlendFactor::ONE, T3DBlendFactor::ONE_MINUS_SRC_ALPHA, T3DBlendEquation::ADD, T3DBlendFactor::ONE, T3DBlendFactor::ZERO };

	bsPresets_[BS_OFF] = CreateBlendState(off);
	bsPresets_[BS_ADDITIVE] = CreateBlendState(additive);
	bsPresets_[BS_STANDARD_ALPHA] = CreateBlendState(standard_alpha);
	bsPresets_[BS_PREMUL_ALPHA] = CreateBlendState(premul_alpha);

	vsPresets_[VS_TEXTURE_COLOR_2D] = CreateVertexShader(glsl_vsTexCol, hlslVsTexCol);
	vsPresets_[VS_COLOR_2D] = CreateVertexShader(glsl_vsCol, hlslVsCol);

	fsPresets_[FS_TEXTURE_COLOR_2D] = CreateFragmentShader(glsl_fsTexCol, hlslFsTexCol);
	fsPresets_[FS_COLOR_2D] = CreateFragmentShader(glsl_fsCol, hlslFsCol);

	ssPresets_[SS_TEXTURE_COLOR_2D] = CreateShaderSet(vsPresets_[VS_TEXTURE_COLOR_2D], fsPresets_[FS_TEXTURE_COLOR_2D]);
	ssPresets_[SS_COLOR_2D] = CreateShaderSet(vsPresets_[VS_COLOR_2D], fsPresets_[FS_COLOR_2D]);
}

Thin3DContext::~Thin3DContext() {
	for (int i = 0; i < VS_MAX_PRESET; i++) {
		vsPresets_[i]->Release();
	}
	for (int i = 0; i < FS_MAX_PRESET; i++) {
		fsPresets_[i]->Release();
	}
	for (int i = 0; i < BS_MAX_PRESET; i++) {
		bsPresets_[i]->Release();
	}
	for (int i = 0; i < SS_MAX_PRESET; i++) {
		ssPresets_[i]->Release();
	}
}