#ifndef AK_LIB_CONTAINER_H
#define AK_LIB_CONTAINER_H

#include "AKLib/common.h"
#include "AKLib/Gui/Component.h"

NSB(AKLib)

class AKLIB_API Container : public Component
{
public:
	inline Container(Point2D<float> pos, Point2D<float> dimensions)
		: Component(pos, dimensions, NULL)
	{
	}
	
	inline ~Container()
	{
	}

	void Render() 
	{
		for (Component* c : children) 
		{
			c->Render();
		}
	}

	inline void Update(InputEvent* event)
	{
		for (Component* c : children)
		{
			if (event->type == InputType::TYPE_MOUSE_PRESS || event->type == InputType::TYPE_MOUSE_MOVE)
			{
				Point2D<float> cpos = c->GetAbsolutePos();
				Point2D<float> cdim = c->GetDimensions();

				MouseMoveEvent* e = (MouseMoveEvent*)event;

				if (e->pos.x > cpos.x &&
					e->pos.y > cpos.y &&
					e->pos.x < (cpos.x + cdim.w) &&
					e->pos.y < (cpos.y + cdim.h))
				{
					c->Update(event);
				}
			}
		}
	}

	inline void AddChild(Component* component) 
	{
		children.push_back(component);
	}

private:
};

NSE()

#endif // AK_LIB_CONTAINER_H