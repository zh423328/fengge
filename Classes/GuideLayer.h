//////////////////////////////////////////////////////////////////////////
//引导层

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;

//引导层
class CGuideLayer : public CCLayer
{
public:
	CGuideLayer();
	~CGuideLayer();

	virtual bool init();


	virtual void onEnter();
	virtual void onExit();


	void SetSencil(float nRadius,float nX,float nY);

	void closeCallback(CCObject * pSender);


	CREATE_FUNC(CGuideLayer);

private:
	CCClippingNode *m_pCClippingNode;
};
