#include "edit/operators/color/conversion/Oklab_cvt.hpp"

namespace OklabCvt {
/**
 * @brief Convert a RGB value to Oklab value.
 * Adapted from https://bottosson.github.io/posts/oklab/
 *
 * @param rgb
 * @return Oklab
 */
Oklab LinearRGB2Oklab(const cv::Vec3f& rgb) {
  // RGB to LMS (via linear transformation)
  float r = rgb[0], g = rgb[1], b = rgb[2];

  float l   = 0.4122214708f * r + 0.5363325363f * g + 0.0514459929f * b;
  float m   = 0.2119034982f * r + 0.6806995451f * g + 0.1073969566f * b;
  float s   = 0.0883024619f * r + 0.2817188376f * g + 0.6299787005f * b;

  // Non-linear transformation (cube root)
  float l_  = std::cbrtf(l);
  float m_  = std::cbrtf(m);
  float s_  = std::cbrtf(s);

  // LMS to Oklab
  float L   = 0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_;
  float a   = 1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_;
  float l_b = 0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_;

  return {L, a, l_b};
}

/**
 * @brief Convert a Oklab value back to Oklab value.
 * Adapted from https://bottosson.github.io/posts/oklab/
 *
 * @param rgb
 * @return Oklab
 */
cv::Vec3f Oklab2LinearRGB(const Oklab& lab) {
  float L = lab.L, a = lab.a, b = lab.b;

  // Oklab to LMS (non-linear)
  float l_    = L + 0.3963377774f * a + 0.2158037573f * b;
  float m_    = L - 0.1055613458f * a - 0.0638541728f * b;
  float s_    = L - 0.0894841775f * a - 1.2914855480f * b;

  // LMS to linear (cube)
  float l     = l_ * l_ * l_;
  float m     = m_ * m_ * m_;
  float s     = s_ * s_ * s_;

  // LMS to RGB
  float r     = +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s;
  float g     = -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s;
  float rgb_b = -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s;

  return {r, g, rgb_b};
}
};  // namespace OklabCvt