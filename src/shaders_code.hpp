#ifndef _H_SHADERS_CODE
#define _H_SHADERS_CODE

const char* shaders_code = R"(
  struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR;
  };

  PSInput VSMain(float4 position : POSITION, float4 color : COLOR) {
    PSInput result;

    result.position = position;
    result.color = color;

    return result;
  }

  float4 PSMain(PSInput input) : SV_TARGET {
    return input.color;
  }
)";

#endif /* _H_SHADERS_CODE */

