#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>
#include "Parser.h"

using namespace std;

void extractElementsFromString(vector<float> &elements, string text, int n)
{
	stringstream text_ss;
	text_ss << text;

	for (int i = 0; i < n; i++)
	{
		int j;
		text_ss >> j;
		elements.push_back(j);
	}
}

Parser::Parser(char *filename)
{

	//INIT Scene
	this->scene = * new DemoScene();

	// Read XML from file

	doc=new TiXmlDocument( filename );
	bool loadOkay = doc->LoadFile();

	if ( !loadOkay )
	{
		printf( "Could not load file '%s'. Error='%s'. Exiting.\n", filename, doc->ErrorDesc() );
		exit( 1 );
	}

	TiXmlElement* yafElement= doc->FirstChildElement( "yaf" );

	if (yafElement == NULL)
	{
		printf("Main yaf block element not found! Exiting!\n");
		exit(1);
	}

	globalsElement = yafElement->FirstChildElement( "globals" );
	if(!globalsElement)
		throw "Error parsing globals";
	string background, drawmode, shading, cullface, cullorder;
	background = globalsElement->Attribute("background");
	drawmode = globalsElement->Attribute("drawmode");
	shading = globalsElement->Attribute("shading");
	cullface = globalsElement->Attribute("cullface");
	cullorder = globalsElement->Attribute("cullorder");
	if(background.empty() || drawmode.empty() || shading.empty() || cullface.empty() || cullorder.empty())
		throw "Error parsing globals attributes";

	if(drawmode == "fill")
		this->scene.drawMode = GL_FILL;
	else if(drawmode == "line")
		this->scene.drawMode = GL_LINE;
	else if(drawmode == "point")
		this->scene.drawMode = GL_POINT;
	else
		throw "Invalid drawmode";

	if(shading == "flat")
		this->scene.shadeModel = GL_FLAT;
	else if(shading == "gouraud")
		this->scene.shadeModel = GL_SMOOTH;
	else
		throw "Invalid shading";

	if(cullface == "none")
		this->scene.cullface = false;
	else {
		this->scene.cullface = true;
		if(cullface=="back")
			this->scene.cullfaceMode = GL_BACK;
		else if(cullface=="front")
			this->scene.cullfaceMode = GL_FRONT;
		else if(cullface=="both")
			this->scene.cullfaceMode = GL_FRONT_AND_BACK;
		else
			throw "Invalid cullface";
	}

	if(cullorder == "CCW")
		this->scene.cullorder = GL_CCW;
	else if(cullorder == "CW")
		this->scene.cullorder = GL_CW;
	else
		throw "Invalid cullorder";

	camerasElement = yafElement->FirstChildElement( "cameras" );
	if(!camerasElement)
		throw "Error parsing cameras";
	string initialCamera = camerasElement->Attribute("initial");
	if(initialCamera.empty())
		throw "Error parsing initial camera";
	TiXmlElement* camera = findChildByAttribute(camerasElement, "id", initialCamera.c_str());

	if(!camera)
		throw "Initial camera not declared";
	cout << "Cameras" << endl;
	cout << "\t Initial: " << initialCamera << endl;
	camera = camerasElement->FirstChildElement();

	while(camera)
	{
		float near, far, left, right, top, bottom, angle, pos_x, pos_y, pos_z, target;
		string id = camera->Attribute("id"),type = camera->Value(), pos;		
		vector<float> pos_vector;// = {0,0,0};

		camera->QueryFloatAttribute("near", &near);
		camera->QueryFloatAttribute("far", &far);

		cout << endl << "\t ID: " << id << endl;
		cout << "\t Type: " << type << endl;
		cout << "\t Near: " << near << endl;
		cout << "\t Far: " << far << endl;

		if (type == "perspective")
		{
			camera->QueryFloatAttribute("angle", &angle);
			pos = camera->Attribute("pos");

			extractElementsFromString(pos_vector, pos, 3);
			/*stringstream pos_ss;
			pos_ss << pos;
			pos_ss >> pos_x >> pos_y >> pos_z;*/
			camera->QueryFloatAttribute("target", &target);

			//print camera attributes
			cout << "\t Angle: " << angle << endl;
			cout << "\t Pos: ";
			
			for (int i = 0; i < pos_vector.size(); i++)
				cout << pos_vector[i] << " ";
			cout << endl;
			cout << "\t Target: " << target << endl;
			
		}
		else if (type == "ortho")
		{
			camera->QueryFloatAttribute("left", &left);
			camera->QueryFloatAttribute("right", &right);
			camera->QueryFloatAttribute("top", &top);
			camera->QueryFloatAttribute("bottom", &bottom);

			//print camera attributes
			cout << "\t Left: " << left << endl;
			cout << "\t Right: " << right << endl;
			cout << "\t Top: " << top << endl;
			cout << "\t Bottom: " << bottom << endl;
		}


		camera = camera->NextSiblingElement();
	}


	//TODO: more than one camera


	lightingElement = yafElement->FirstChildElement( "lighting" );
	if(!lightingElement)
		throw "Error parsing lighting";
	bool doublesided, local, enabled;
	string ambient;
	doublesided = to_bool(lightingElement->Attribute("doublesided"));
	local = to_bool(lightingElement->Attribute("local"));
	enabled = to_bool(lightingElement->Attribute("enabled"));
	ambient= lightingElement->Attribute("ambient");
	if(ambient.empty())
		throw "Error parsing lighting attributes";
	TiXmlElement *child = lightingElement->FirstChildElement();
	cout << "Lighting" << endl;
	cout << "\tDoublesided: " << boolalpha << doublesided << endl;
	cout << "\tLocal: " << boolalpha << local << endl;
	cout << "\tEnabled: " << boolalpha << enabled << endl;
	cout << "\tAmbient: " << ambient << endl;


	texturesElement = yafElement->FirstChildElement( "textures" );
	if(!texturesElement)
		throw "Error parsing textures";
	string id, file_name;
	cout << "Textures" << endl;


	appearancesElement = yafElement->FirstChildElement( "appearances" );
	if(!appearancesElement)
		throw "Error parsing appearances";
	cout << "Appearances" << endl;


	graphElement = yafElement->FirstChildElement( "graph" );
	if(!graphElement)
		throw "Error parsing graph";
	string rootid;
	rootid = graphElement->Attribute("rootid");
	if(rootid.empty())
		throw "Error parsing graph attributes";
	cout << "Graph" << endl;
	cout << "\tRoot ID" << rootid << endl;
}
/*	// Init
// An example of well-known, required nodes

if (initElement == NULL)
printf("Init block not found!\n");
else
{
printf("Processing init:\n");
// frustum: example of a node with individual attributes
TiXmlElement* frustumElement=initElement->FirstChildElement("frustum");
if (frustumElement)
{
float near,far;

if (frustumElement->QueryFloatAttribute("near",&near)==TIXML_SUCCESS && 
frustumElement->QueryFloatAttribute("far",&far)==TIXML_SUCCESS
)
printf("  frustum attributes: %f %f\n", near, far);
else
printf("Error parsing frustum\n");
}
else
printf("frustum not found\n");


// translate: example of a node with an attribute comprising several float values
// It shows an example of extracting an attribute's value, and then further parsing that value 
// to extract individual values
TiXmlElement* translateElement=initElement->FirstChildElement("translate");
if (translateElement)
{
char *valString=NULL;
float x,y,z;

valString=(char *) translateElement->Attribute("xyz");

if(valString && sscanf(valString,"%f %f %f",&x, &y, &z)==3)
{
printf("  translate values (XYZ): %f %f %f\n", x, y, z);
}
else
printf("Error parsing translate");
}
else
printf("translate not found\n");		

// repeat for each of the variables as needed
}

// Other blocks could be validated/processed here


// graph section
if (graphElement == NULL)
printf("Graph block not found!\n");
else
{
char *prefix="  -";
TiXmlElement *node=graphElement->FirstChildElement();

while (node)
{
printf("Node id '%s' - Descendants:\n",node->Attribute("id"));
TiXmlElement *child=node->FirstChildElement();
while (child)
{
if (strcmp(child->Value(),"Node")==0)
{
// access node data by searching for its id in the nodes section

TiXmlElement *noderef=findChildByAttribute(nodesElement,"id",child->Attribute("id"));

if (noderef)
{
// print id
printf("  - Node id: '%s'\n", child->Attribute("id"));

// prints some of the data
printf("    - Material id: '%s' \n", noderef->FirstChildElement("material")->Attribute("id"));
printf("    - Texture id: '%s' \n", noderef->FirstChildElement("texture")->Attribute("id"));

// repeat for other leaf details
}
else
printf("  - Node id: '%s': NOT FOUND IN THE NODES SECTION\n", child->Attribute("id"));

}
if (strcmp(child->Value(),"Leaf")==0)
{
// access leaf data by searching for its id in the leaves section
TiXmlElement *leaf=findChildByAttribute(leavesElement,"id",child->Attribute("id"));

if (leaf)
{
// it is a leaf and it is present in the leaves section
printf("  - Leaf id: '%s' ; type: '%s'\n", child->Attribute("id"), leaf->Attribute("type"));

// repeat for other leaf details
}
else
printf("  - Leaf id: '%s' - NOT FOUND IN THE LEAVES SECTION\n",child->Attribute("id"));
}

child=child->NextSiblingElement();
}
node=node->NextSiblingElement();
}
}

}*/

Parser::~Parser()
{
	delete(doc);
}

//-------------------------------------------------------

TiXmlElement *Parser::findChildByAttribute(TiXmlElement *parent,const char * attr, const char *val)
	// Searches within descendants of a parent for a node that has an attribute _attr_ (e.g. an id) with the value _val_
	// A more elaborate version of this would rely on XPath expressions
{
	TiXmlElement *child=parent->FirstChildElement();
	int found=0;

	while (child && !found)
		if (child->Attribute(attr) && strcmp(child->Attribute(attr),val)==0)
			found=1;
		else
			child=child->NextSiblingElement();

	return child;
}

bool to_bool(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}