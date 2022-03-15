#ifndef AK_LIB_COMPONENT_H
#define AK_LIB_COMPONENT_H

#include "AKLib/common.h"
#include "AKLib/Vector.h"
#include "AKLib/Point.h"

NSB(AKLib)

enum InputType
{
	TYPE_KEY_PRESS,
	TYPE_MOUSE_MOVE,
	TYPE_MOUSE_PRESS
};

struct InputEvent
{
	InputType type;
};

struct MouseMoveEvent : public InputEvent
{
	Point2D<float> pos;
};

struct MousePressEvent : public InputEvent
{
	Point2D<float> pos;
	int button;
	int down;
	int mods;
};

struct KeyPressEvent : public InputEvent
{
	int key;
	int down;
	int mods;
};

class AKLIB_API Component
{
public:
	inline Component(Point2D<float> pos, Point2D<float> dimensions, Component* parent) 
		: children{ }, parent{ parent }, pos{ pos }, dimensions{ dimensions }
	{
	}

	virtual void Render() = 0;
	virtual void Update(InputEvent* event) = 0;

	inline Point2D<float> GetPos() 
	{
		return pos;
	}

	inline Point2D<float> GetAbsolutePos() 
	{
		if (parent != NULL)
		{
			Point2D<float> pPos = parent->GetAbsolutePos();
			return { pos.x + pPos.x, pos.y + pPos.y };
		}

		return pos;
	}

	inline Point2D<float> GetDimensions() 
	{
		return dimensions;
	}

	inline void AddChild(Component* child) 
	{
		children.push_back(child);
		child->parent = this;
	}
protected:
	Vector<Component*> children;
	Component* parent;
	Point2D<float> pos;
	Point2D<float> dimensions;
};

NSE()

#endif // AK_LIB_COMPONENT_H