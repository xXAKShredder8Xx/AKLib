#include "AKLib/Allocator.h"
#include "AKLib/File.h"
#include <string.h>

NSB(AKLib)

AKL_INLINE XMLAttribute::XMLAttribute(String key, String value)
	: key(key), value(value)
{
}

AKL_INLINE XMLAttribute::XMLAttribute(const XMLAttribute& other)
	: key(other.key), value(other.value)
{
}

AKL_INLINE XMLAttribute& XMLAttribute::operator = (const XMLAttribute& other)
{
	key = other.key;
	value = other.value;
	return *this;
}

AKL_INLINE XMLAttribute::~XMLAttribute()
{
}

AKL_INLINE XMLNode::XMLNode(XMLNode* parent)
	: tag{ "" }, inner_text{ "" }, parent{ parent }, attributes{ }, children{ }
{
	if (parent != NULL)
	{
		parent->children.push_back(this);
	}
}

AKL_INLINE XMLNode::XMLNode(String tag, XMLNode* parent)
	: tag{ tag }, inner_text{ "" }, parent{ parent }, attributes{ }, children{ }
{
	if (parent != NULL)
	{
		parent->children.push_back(this);
	}
}

AKL_INLINE XMLNode::XMLNode(String tag, String inner_text, XMLNode* parent)
	: tag{ tag }, inner_text{ inner_text }, parent{ parent }, attributes{ }, children{ }
{
	if (parent != NULL)
	{
		parent->children.push_back(this);
	}
}

AKL_INLINE XMLNode::~XMLNode()
{
	//printf("Tag: %s\n", tag.c_str());

	for (XMLNode* child : children)
	{
		delete child;
	}
}

AKL_INLINE XMLNode* XMLNode::GetChild(int index)
{
	return children[index];
}

AKL_INLINE XMLNode* XMLNode::GetChild(const String& tag)
{
	for (XMLNode* node : children)
	{
		if (node->tag == tag)
			return node;
	}

	return NULL;
}

AKL_INLINE Vector<XMLNode*> XMLNode::GetChildren(const String& tag)
{
	Vector<XMLNode*> found;

	for (XMLNode* node : children)
	{
		if (node->tag == tag)
		{
			found.push_back(node);
		}
	}

	return found;
}

AKL_INLINE XMLNode* XMLNode::GetChildRecursive(const String& tag, XMLNode* root)
{
	if (root == NULL) root = this;

	XMLNode* node = NULL;

	for (XMLNode* temp : root->children)
	{
		//printf("Comparing '%s' with '%s'\n", temp->tag.c_str(), tag.c_str());
		if (temp->tag == tag)
			return temp;

		XMLNode* t = GetChildRecursive(tag, temp);

		if (t != NULL)
		{
			node = t;
			break;
		}
	}

	return node;
}

AKL_INLINE void XMLNode::GetChildrenRecursive(const String& tag, Vector<XMLNode*>& vec,
	XMLNode* root)
{
	if (root == NULL) root = this;

	XMLNode* node = NULL;

	for (XMLNode* temp : root->children)
	{
		//printf("Comparing '%s' with '%s'\n", temp->tag.c_str(), tag.c_str());
		if (temp->tag == tag)
			vec.push_back(temp);

		GetChildrenRecursive(tag, vec, temp);
	}
}

AKL_INLINE void XMLNode::AddChild(XMLNode* child)
{
	child->parent = this;
	if (children.find(child) == children.end())
		children.push_back(child);
}

AKL_INLINE XMLNode* XMLNode::AddChild(String tag, String text)
{
	XMLNode* node = new XMLNode(tag, text, this);
	children.push_back(node);
	return node;
}

AKL_INLINE void XMLNode::RemoveChild(XMLNode* child)
{
	Vector<XMLNode*>::const_iterator pos = NULL;
	if ((pos = children.find(child)).get() != NULL)
	{
		if (children.erase(pos))
			delete child;
	}
}

AKL_INLINE void XMLNode::Write(FILE* file, int indent, int times)
{
	for (XMLNode* child : children)
	{
		if (times > 0)
			fprintf(file, "%*s", indent * times, " ");

		fprintf(file, "<%s", child->tag.c_str());

		for (XMLAttribute& attr : child->attributes)
		{
			if (attr.value == "")
				continue;
			fprintf(file, " %s=\"%s\"", attr.key.c_str(), attr.value.c_str());
		}

		if (child->children.size() == 0 && !child->inner_text)
		{
			fprintf(file, " />\n");
		}
		else
		{
			fprintf(file, ">");
			if (child->children.size() == 0)
			{
				fprintf(file, "%s</%s>\n", child->inner_text.c_str(), child->tag.c_str());
			}
			else
			{
				fprintf(file, "\n");
				child->Write(file, indent, times + 1);
				if (times > 0)
				{
					fprintf(file, "%*s", indent * times, " ");
				}

				fprintf(file, "</%s>\n", child->tag.c_str());
			}
		}
	}
}

AKL_INLINE void XMLNode::ToString(String& str, int indent, int times)
{
	for (XMLNode* child : children)
	{
		if (times > 0)
			str.Append(String::Format("%*s", indent * times, " "));

		str.Append(String::Format("<%s", child->tag.c_str()));

		for (XMLAttribute& attr : child->attributes)
		{
			if (attr.value == "")
				continue;
			str.Append(String::Format(" %s=\"%s\"", attr.key.c_str(), attr.value.c_str()));
		}

		if (child->children.size() == 0 && !child->inner_text)
		{
			str.Append(" />\n");
		}
		else
		{
			str.Append(">");
			if (child->children.size() == 0)
			{
				str.Append(String::Format("%s</%s>\n", child->inner_text.c_str(), child->tag.c_str()));
			}
			else
			{
				str.Append("\n");
				child->ToString(str, indent, times + 1);
				if (times > 0)
				{
					str.Append(String::Format("%*s", indent * times, " "));
				}

				str.Append(String::Format("</%s>\n", child->tag.c_str()));
			}
		}
	}
}

AKL_INLINE void XMLNode::AddAttribute(XMLAttribute attrib)
{
	attributes.push_back(attrib);
}

AKL_INLINE void XMLNode::AddAttribute(String key, String value)
{
	attributes.push_back(XMLAttribute(key, value));
}

AKL_INLINE XMLAttribute* XMLNode::GetAttribute(int index)
{
	return &attributes[index];
}

AKL_INLINE XMLAttribute* XMLNode::GetAttribute(const String& key)
{
	for (XMLAttribute& attr : attributes)
	{
		if (attr.key == key)
		{
			return &attr;
		}
	}

	return NULL;
}

AKL_INLINE String XMLNode::GetAttributeVal(int index)
{
	return GetAttribute(index)->value;
}

AKL_INLINE String XMLNode::GetAttributeVal(const String& key)
{
	XMLAttribute* attr = GetAttribute(key);
	if (attr != NULL)
		return attr->value;

	return "";
}

/*
* Beginning of XMLTypeNode<int32_t>
*/

AKL_INLINE XMLTypeNode<int32_t>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "0", parent }, value{ 0 }
{
	AddAttribute("type", "int");
}

AKL_INLINE XMLTypeNode<int32_t>::XMLTypeNode(int32_t value, XMLNode* parent)
	: XMLNode{ "value", "", parent }
{
	AddAttribute("type", "int");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<int32_t>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE int32_t XMLTypeNode<int32_t>::GetValue()
{
	return value;
}

template <>
AKL_INLINE void XMLTypeNode<int32_t>::SetValue(const int32_t& val)
{
	value = val;
	String::Format(inner_text, "%d", val);
}

/*
* Beginning of XMLTypeNode<uint32_t>
*/

AKL_INLINE XMLTypeNode<uint32_t>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "0", parent }, value{ 0 }
{
	AddAttribute("type", "uint");
}

AKL_INLINE XMLTypeNode<uint32_t>::XMLTypeNode(uint32_t value, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }
{
	AddAttribute("type", "uint");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<uint32_t>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE uint32_t XMLTypeNode<uint32_t>::GetValue()
{
	return value;
}

template <>
AKL_INLINE void XMLTypeNode<uint32_t>::SetValue(const uint32_t& val)
{
	value = val;
	String::Format(inner_text, "%u", val);
}

/*
* Beginning of XMLTypeNode<int64_t>
*/

AKL_INLINE XMLTypeNode<int64_t>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "0", parent }, value{ 0 }
{
	AddAttribute("type", "long");
}

AKL_INLINE XMLTypeNode<int64_t>::XMLTypeNode(int64_t value, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }
{
	AddAttribute("type", "long");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<int64_t>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE int64_t XMLTypeNode<int64_t>::GetValue()
{
	return value;
}

template <>
AKL_INLINE void XMLTypeNode<int64_t>::SetValue(const int64_t& val)
{
	value = val;
	String::Format(inner_text, "%I64d", val);
}

/*
* Beginning of XMLTypeNode<uint64_t>
*/

AKL_INLINE XMLTypeNode<uint64_t>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "0", parent }, value{ 0 }
{
	AddAttribute("type", "ulong");
}

AKL_INLINE XMLTypeNode<uint64_t>::XMLTypeNode(uint64_t value, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }
{
	AddAttribute("type", "ulong");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<uint64_t>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE uint64_t XMLTypeNode<uint64_t>::GetValue()
{
	return value;
}

template <>
AKL_INLINE void XMLTypeNode<uint64_t>::SetValue(const uint64_t& val)
{
	value = val;
	String::Format(inner_text, "%I64u", val);
}

/*
* Beginning of XMLTypeNode<float>
*/

AKL_INLINE XMLTypeNode<float>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "0", parent }, value{ 0.0f }, precision{ 0 }
{
	AddAttribute("type", "float");
}

AKL_INLINE XMLTypeNode<float>::XMLTypeNode(float value, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }, precision{ 0 }
{
	AddAttribute("type", "float");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<float>::XMLTypeNode(float value, int prec, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }, precision{ prec }
{
	if (!(value < 0.0f && value > 0.0f))
	{
		precision = 0;
	}

	AddAttribute("type", "float");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<float>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE float XMLTypeNode<float>::GetValue()
{
	return value;
}

template <>
AKL_INLINE void XMLTypeNode<float>::SetValue(const float& val)
{
	value = val;
	String::FormatFloat(inner_text, precision, val);
}

AKL_INLINE void XMLTypeNode<float>::SetPrecision(int prec)
{
	precision = prec;
}

AKL_INLINE int XMLTypeNode<float>::GetPrecision()
{
	return precision;
}

/*
* Beginning of XMLTypeNode<double>
*/

AKL_INLINE XMLTypeNode<double>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "0", parent }, value{ 0.0 }, precision{ 0 }
{
	AddAttribute("type", "double");
}

AKL_INLINE XMLTypeNode<double>::XMLTypeNode(double value, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }, precision{ 0 }
{
	AddAttribute("type", "double");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<double>::XMLTypeNode(double value, int prec, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }, precision{ prec }
{
	if (value == 0.0)
	{
		precision = 0;
	}

	AddAttribute("type", "double");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<double>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE double XMLTypeNode<double>::GetValue()
{
	return value;
}

template <>
AKL_INLINE void XMLTypeNode<double>::SetValue(const double& val)
{
	value = val;
	String::FormatDouble(inner_text, precision, val);
}

AKL_INLINE void XMLTypeNode<double>::SetPrecision(int prec)
{
	precision = prec;
}

AKL_INLINE int XMLTypeNode<double>::GetPrecision()
{
	return precision;
}

/*
* Beginning of XMLTypeNode<String>
*/

AKL_INLINE XMLTypeNode<String>::XMLTypeNode(XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ }
{
	AddAttribute("type", "string");
}

AKL_INLINE XMLTypeNode<String>::XMLTypeNode(String value, XMLNode* parent)
	: XMLNode{ "value", "", parent }, value{ value }
{
	AddAttribute("type", "string");
	SetValue(value);
}

AKL_INLINE XMLTypeNode<String>::~XMLTypeNode()
{
	//printf("Tag: %s\n", tag.c_str());
}

AKL_INLINE String XMLTypeNode<String>::GetValue()
{
	return inner_text;
}

template <>
AKL_INLINE void XMLTypeNode<String>::SetValue(const String& val)
{
	inner_text = val;
}

AKL_INLINE XMLDocument::XMLDocument()
	: root{ new XMLNode(NULL) }
{
}

AKL_INLINE XMLDocument::XMLDocument(const char* path)
	: root{ NULL }
{
	Load(path);
}

AKL_INLINE XMLDocument::~XMLDocument()
{
	if (root != NULL)
	{
		delete root;
		root = NULL;
	}
}

AKL_INLINE bool XMLDocument::Load(const char* path)
{
	if (root != NULL) delete root;

	File file(path, FMODE_READ);

	file.Open();

	if (!file.IsOpen())
	{
		fprintf(stderr, "Could not load file '%s'\n", path);
		return false;
	}

	size_t size = file.Size();

	Vector<char> buffer((size_t)size + 1);

	file.Rewind();

	file.Read(buffer.data());

	file.Close();

	buffer[size] = '\0';

	root = new XMLNode("", "", NULL);

	XMLNode* currentNode = root;

	int i = 0;
	int lexi = 0;
	Vector<char> lex(512);

	while (buffer[i] != '\0')
	{
		if (buffer[i] == '<')
		{
			lex[lexi] = '\0';

			// Inner text
			if (lexi > 0)
			{
				if (currentNode == NULL)
				{
					fprintf(stderr, "Text outside of document\n");
					return false;
				}

				currentNode->inner_text = lex.data();

				lexi = 0;
			}

			// End of node
			if (buffer[i + 1] == '/')
			{
				i += 2;

				while (buffer[i] != '>')
				{
					lex[lexi++] = buffer[i++];
				}

				lex[lexi] = '\0';

				if (currentNode == NULL)
				{
					fprintf(stderr, "Already at the root\n");
					return false;
				}

				if (strcmp(currentNode->tag.c_str(), lex.data()))
				{
					fprintf(stderr, "Mismatched tags (%s != %s)\n", currentNode->tag.c_str(), lex.data());
					return false;
				}

				currentNode = currentNode->parent;
				i++;
				continue;
			}

			// Special nodes
			if (buffer[i + 1] == '!')
			{
				while (buffer[i] != ' ' && buffer[i] != '>')
				{
					lex[lexi++] = buffer[i++];
				}
				lex[lexi] = '\0';

				// Comments
				if (String::Compare(lex.data(), strlen(lex.data()), "<!--", 4))
				{
					lex[lexi] = '\0';

					while (!String::EndsWith(lex.data(), strlen(lex.data()), "-->", 3))
					{
						lex[lexi++] = buffer[i++];
						lex[lexi] = '\0';
					}

					continue;
				}
			}

			// Declaration tags
			if (buffer[i + 1] == '?')
			{
				while (buffer[i] != ' ' && buffer[i] != '>')
				{
					lex[lexi++] = buffer[i++];
				}
				lex[lexi] = '\0';

				// This is the XML declaration
				if (String::Compare(lex.data(), strlen(lex.data()), "<?xml", 5))
				{
					lexi = 0;
					XMLNode* desc = new XMLNode("", "", NULL);
					ParseAttrs(buffer, i, lex, lexi, desc);

					version = desc->GetAttributeVal("version");
					encoding = desc->GetAttributeVal("encoding");

					delete desc;

					continue;
				}
			}

			// Set current node
			currentNode = new XMLNode("", "", currentNode);

			// Start tag
			i++;

			if (ParseAttrs(buffer, i, lex, lexi, currentNode) == TAG_INLINE)
			{
				currentNode = currentNode->parent;
				i++;
				continue;
			}

			// Set tag name if none
			lex[lexi] = '\0';

			if (!currentNode->tag)
			{
				currentNode->tag = lex.data();
			}

			lexi = 0;
			i++;
			continue;
		}
		else
		{
			lex[lexi++] = buffer[i++];
		}
	}

	return true;
}

AKL_INLINE bool XMLDocument::Load(String source)
{
	if (root != NULL) delete root;

	Vector<char> buffer(source.Size());

	Allocator<char>::Copy(source.c_str(), buffer.data(), source.Size() + 1);

	root = new XMLNode("", "", NULL);

	XMLNode* currentNode = root;

	int i = 0;
	int lexi = 0;
	Vector<char> lex(512);

	while (buffer[i] != '\0')
	{
		if (buffer[i] == '<')
		{
			lex[lexi] = '\0';

			// Inner text
			if (lexi > 0)
			{
				if (currentNode == NULL)
				{
					fprintf(stderr, "Text outside of document\n");
					return false;
				}

				currentNode->inner_text = lex.data();

				lexi = 0;
			}

			// End of node
			if (buffer[i + 1] == '/')
			{
				i += 2;

				while (buffer[i] != '>')
				{
					lex[lexi++] = buffer[i++];
				}

				lex[lexi] = '\0';

				if (currentNode == NULL)
				{
					fprintf(stderr, "Already at the root\n");
					return false;
				}

				if (strcmp(currentNode->tag.c_str(), lex.data()))
				{
					fprintf(stderr, "Mismatched tags (%s != %s)\n", currentNode->tag.c_str(), lex.data());
					return false;
				}

				currentNode = currentNode->parent;
				i++;
				continue;
			}

			// Special nodes
			if (buffer[i + 1] == '!')
			{
				while (buffer[i] != ' ' && buffer[i] != '>')
				{
					lex[lexi++] = buffer[i++];
				}
				lex[lexi] = '\0';

				// Comments
				if (String::Compare(lex.data(), strlen(lex.data()), "<!--", 4))
				{
					lex[lexi] = '\0';

					while (!String::EndsWith(lex.data(), strlen(lex.data()), "-->", 3))
					{
						lex[lexi++] = buffer[i++];
						lex[lexi] = '\0';
					}

					continue;
				}
			}

			// Declaration tags
			if (buffer[i + 1] == '?')
			{
				while (buffer[i] != ' ' && buffer[i] != '>')
				{
					lex[lexi++] = buffer[i++];
				}
				lex[lexi] = '\0';

				// This is the XML declaration
				if (String::Compare(lex.data(), strlen(lex.data()), "<?xml", 5))
				{
					lexi = 0;
					XMLNode* desc = new XMLNode("", "", NULL);
					ParseAttrs(buffer, i, lex, lexi, desc);

					version = desc->GetAttributeVal("version");
					encoding = desc->GetAttributeVal("encoding");

					delete desc;

					/*if (encoding != "utf-8" && encoding != "UTF-8")
					{
						fprintf(stderr, "XML does not support %s encoding!\n", encoding.c_str());
						return false;
					}*/

					continue;
				}
			}

			// Set current node
			currentNode = new XMLNode("", "", currentNode);

			// Start tag
			i++;
			if (ParseAttrs(buffer, i, lex, lexi, currentNode) == TAG_INLINE)
			{
				currentNode = currentNode->parent;
				i++;
				continue;
			}

			// Set tag name if none
			lex[lexi] = '\0';

			if (!currentNode->tag)
			{
				currentNode->tag = lex.data();
			}

			lexi = 0;
			i++;
			continue;
		}
		else
		{
			lex[lexi++] = buffer[i++];
		}
	}

	return true;
}

AKL_INLINE bool XMLDocument::Write(const char* path, int indent)
{
	FILE* file;
	fopen_s(&file, path, "w");

	if (file == NULL)
	{
		fprintf(stderr, "Could not open file '%s'\n", path);
		return false;
	}

	fprintf(file, "<?xml version=\"%s\" encoding=\"%s\" ?>\n",
		(version) ? version.c_str() : "1.0",
		(encoding) ? encoding.c_str() : "utf-8");

	root->Write(file, indent, 0);

	fclose(file);
	return true;
}

AKL_INLINE XMLDocument::TagType XMLDocument::ParseAttrs(Vector<char>& buffer, int& i,
	Vector<char>& lex, int& lexi, XMLNode* currentNode)
{
	XMLAttribute currentAttribute("", "");
	while (buffer[i] != '>')
	{
		lex[lexi++] = buffer[i++];

		// Tag name
		if (buffer[i] == ' ' && !currentNode->tag)
		{
			lex[lexi] = '\0';
			currentNode->tag = lex.data();
			lexi = 0;
			i++;
			continue;
		}

		// Usually ignore spaces
		if (lex[lexi - 1] == ' ')
		{
			lexi--;
		}

		// Attribute key
		if (buffer[i] == '=')
		{
			lex[lexi] = '\0';
			currentAttribute.key = lex.data();
			lexi = 0;
			continue;
		}

		// Attribute value
		if (buffer[i] == '"')
		{
			if (!currentAttribute.key)
			{
				fprintf(stderr, "Value has no key\n");
				return TAG_START;
			}

			lexi = 0;
			i++;

			while (buffer[i] != '"')
			{
				lex[lexi++] = buffer[i++];
			}
			lex[lexi] = '\0';
			currentAttribute.value = lex.data();
			currentNode->attributes.push_back(currentAttribute);
			currentAttribute.key = "";
			currentAttribute.value = "";
			lexi = 0;
			i++;
			continue;
		}

		// Inline mode
		if (buffer[i - 1] == '/' && buffer[i] == '>')
		{
			lex[lexi] = '\0';

			if (!currentNode->tag)
			{
				currentNode->tag = lex.data();
			}

			i++;
			return TAG_INLINE;
		}
	}

	return TAG_START;
}

AKL_INLINE XMLNode* XMLDocument::SearchForNode(XMLNode* root, const String& tag)
{
	XMLNode* node = NULL;

	for (XMLNode* temp : root->children)
	{
		//printf("Comparing '%s' with '%s'\n", temp->tag.c_str(), tag.c_str());
		if (temp->tag == tag)
			return temp;

		XMLNode* t = SearchForNode(temp, tag);

		if (t != NULL)
		{
			node = t;
			break;
		}
	}

	return node;
}

AKL_INLINE void XMLDocument::AddNode(XMLNode* node) 
{
	root->AddChild(node);
}

NSE()