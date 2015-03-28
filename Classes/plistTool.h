#ifndef PLIST_TOOL_H_
#define PLIST_TOOL_H_

#include "cocos2d.h"
USING_NS_CC;

class CPlistTool
{
public:
	static CPlistTool * sharePlistToolManager();
	
	//获取渲染文理
	CCRenderTexture* getRenderTexture(int widthP,int heightP,CCNode* nodeP,float node_xP,float node_yP);

	//保存图片
	bool createPng(const char* pngName,CCRenderTexture* textureP);
};
#endif