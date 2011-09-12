function onScriptInitXML()
{		
	local xmlFile = xml("helloWorld.xml");
	log(xmlFile.nodeContent().tostring()); //false < RootNode
	log(xmlFile.nodeName()); //rootNode
	xmlFile.nodeFind("noneExist");
	xmlFile.nodeFind("childNode");
	xmlFile.nodeFirstChild();
	log(xmlFile.nodeContent()); //I am a child
	xmlFile.nodeSetContent("Heyho, i am the new Content");
	xmlFile.nodeNext();
	xmlFile.save();
	return 1;
}
addEvent("scriptInit", onScriptInitXML);