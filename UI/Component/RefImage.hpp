#ifndef REFIMAGE_HPP
#define REFIMAGE_HPP
#include <allegro5/bitmap.h>
#include <memory>
#include <string>
#include "Engine/IObject.hpp"

namespace Engine {
    /// <summary>
    /// A simple static image object.
    /// </summary>
    class RefImage : public IObject {
    public:
        // Smart pointer to bitmap.
        std::shared_ptr<ALLEGRO_BITMAP> bmp;
        explicit RefImage(std::string img, float x, float y, float w, float h, float anchorX, float anchorY);
        void Draw() const override;
        int GetBitmapWidth() const;
        int GetBitmapHeight() const;

        void ChangeImageTo(std::string img, int x, int y);
    };
}
#endif // REFIMAGE_HPP
