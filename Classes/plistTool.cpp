#include "plistTool.h"

static CPlistTool * m_pSharePlistManager = NULL;


CPlistTool * CPlistTool::sharePlistToolManager()
{
	if(m_pSharePlistManager == NULL)
	{
		m_pSharePlistManager = new CPlistTool();
	}

	return m_pSharePlistManager;
}

CCRenderTexture* CPlistTool::getRenderTexture( int widthP,int heightP,CCNode* nodeP,float node_xP,float node_yP )
{
	CCRenderTexture *rt = CCRenderTexture::create(widthP, heightP);
	//
	rt->begin();
	nodeP->setPosition(node_xP,node_yP);
	nodeP->visit();
	rt->end();
	//
	return rt;
}

bool CPlistTool::createPng( const char* pngName,CCRenderTexture* textureP )
{
	CCImage * imager = textureP->newCCImage();


	return imager->saveToFile(pngName,false);
}




