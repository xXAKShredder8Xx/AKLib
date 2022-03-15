#ifndef AK_LIB_XML_H
#define AK_LIB_XML_H

#include "AKLib/common.h"
#include "AKLib/String.h"
#include "AKLib/Vector.h"

NSB(AKLib)

struct AKLIB_API XMLAttribute
{
	inline XMLAttribute(String key, String value);
	inline XMLAttribute(const XMLAttribute& other);
	inline XMLAttribute& operator = (const XMLAttribute& other);
	inline ~XMLAttribute();

	String key;
	String value;
};

struct AKLIB_API XMLNode
{
	inline XMLNode(XMLNode* parent);
	inline XMLNode(String tag, XMLNode* parent);
	inline XMLNode(String tag, String inner_text, XMLNode* parent);
	inline ~XMLNode();

	inline XMLNode* GetChild(int index);
	inline XMLNode* GetChild(const String& tag);
	inline Vector<XMLNode*> GetChildren(const String& tag);
	inline XMLNode* GetChildRecursive(const String& tag, XMLNode* root = NULL);
	inline void GetChildrenRecursive(const String& tag, Vector<XMLNode*>& vec, XMLNode* root = NULL);
	inline void AddChild(XMLNode* node);
	inline XMLNode* AddChild(String tag, String text = "");
	inline void RemoveChild(XMLNode* node);
	inline void Write(FILE* file, int indent = 4, int times = 0);
	inline void ToString(String& str, int indent = 4, int times = 0);

	inline void AddAttribute(XMLAttribute attrib);
	inline void AddAttribute(String key, String value);
	inline XMLAttribute* GetAttribute(int index);
	inline XMLAttribute* GetAttribute(const String& key);
	inline String GetAttributeVal(int index);
	inline String GetAttributeVal(const String& key);

	String tag;
	String inner_text;
	XMLNode* parent;
	Vector<XMLAttribute> attributes;
	Vector<XMLNode*> children;
};

template <typename T>
struct AKLIB_API XMLTypeNode : public XMLNode
{
	inline XMLTypeNode(XMLNode* parent);
	inline XMLTypeNode(T val, XMLNode* parent);
	inline XMLTypeNode(T val, int prec, XMLNode* parent);
	inline ~XMLTypeNode();
	inline T GetValue();
	inline void SetValue(const T& val);
	inline void SetPrecision(int prec);
	inline int GetPrecision();

	T value;
	int precision;
};

struct AKLIB_API XMLDocument
{
	enum TagType : uint8_t
	{
		TAG_START,
		TAG_INLINE
	};

	inline XMLDocument();
	inline XMLDocument(const char* path);
	inline ~XMLDocument();

	inline bool Load(const char* path);
	inline bool Load(String source);
	inline bool Write(const char* path, int indent = 4);
	inline TagType ParseAttrs(Vector<char>& buffer, int& i, Vector<char>& lex, int& lexi, XMLNode* currentNode);

	inline XMLNode* SearchForNode(XMLNode* root, const String& tag);
	
	inline void AddNode(XMLNode* node);

	XMLNode* root;
	String version;
	String encoding;
};

struct AKLIB_API XMLSerial
{
	virtual XMLNode* Serialize() = 0;
	virtual void Deserialize(XMLNode* node) = 0;
};

NSE()

#define AKL_INLINE inline
#include "AKLib/XML/Xml.inl"

#endif // AK_LIB_XML_H