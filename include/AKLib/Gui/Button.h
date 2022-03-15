#ifndef AK_LIB_BUTTON_H
#define AK_LIB_BUTTON_H

#include "AKLib/common.h"
#include "AKLib/Gui/Component.h"

NSB(AKLib)

class AKLIB_API Button : public Component
{
public:
	Button(Point2D<float> pos, Point2D<float> dim = { 100.0f, 25.0f }, Component* parent = NULL)
		: Component(pos, dim, parent)
	{
	}

	~Button()
	{
	}

	inline void Render() override 
	{
	}

	void Update(InputEvent* event) override 
	{
		if (event->type == InputType::TYPE_MOUSE_PRESS) 
		{
			MousePressEvent* e = (MousePressEvent*)event;

			if (e->down == true)
			{
				printf("Button Pressed at %f, %f!\n", e->pos.x, e->pos.y);
			}
			else 
			{
				printf("Button Released at %f, %f!\n", e->pos.x, e->pos.y);
			}
		}
		else if (event->type == InputType::TYPE_MOUSE_MOVE)
		{
			MouseMoveEvent* e = (MouseMoveEvent*)event;
			
			printf("Mouse is hovering over button at %f, %f!\n", e->pos.x, e->pos.y);
		}
	}
};

NSE()



#endif // AK_LIB_BUTTON_H