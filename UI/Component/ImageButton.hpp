#ifndef IMAGEBUTTON_HPP
#define IMAGEBUTTON_HPP
#include <allegro5/bitmap.h>
#include <functional>
#include <memory>
#include <string>

#include "Engine/IControl.hpp"
#include "Image.hpp"

namespace Engine {
	/// <summary>
	/// A clickable button, changes image when mouse move.
	/// </summary>
	class ImageButton: public Image, public IControl {
	protected:
		bool isMouseIn = false;
		std::shared_ptr<ALLEGRO_BITMAP> imgOut;
		std::shared_ptr<ALLEGRO_BITMAP> imgIn;
		std::function<void()> OnClickCallback;
	public:
		// Whether the button can be pressed.
		bool Enabled = true;
		/// <summary>
		/// Construct a image object.
		/// </summary>
		/// <param name="img">The mouse-out image path in 'Resource/images/'</param>
		/// <param name="imgIn">The mouse-in image path in 'Resource/images/'</param>
		/// <param name="lane">X-coordinate.</param>
		/// <param name="y">Y-coordinate.</param>
		/// <param name="w">Width of the image, 0 indicates original size.</param>
		/// <param name="h">Height of the image, 0 indicates original size.</param>
		/// <param name="anchorX">The centerX of the object. (0, 0) means top-left, while (1, 0) means top-right.</param>
		/// <param name="anchorY">The centerY of the object. (0, 1) means bottom-left, while (1, 1) means bottom-right.</param>
		explicit ImageButton(std::string img, std::string imgIn, float x, float y, float w = 0, float h = 0, float anchorX = 0, float anchorY = 0);
		void SetOnClickCallback(std::function<void(void)> onClickCallback);
		void OnMouseDown(int button, int mx, int my) override;
		void OnMouseMove(int mx, int my) override;
	};
}
#endif // IMAGEBUTTON_HPP
