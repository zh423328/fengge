#include "GuideLayer.h"


CGuideLayer::CGuideLayer()
{
	m_pCClippingNode = NULL;
}

CGuideLayer::~CGuideLayer()
{

}

void CGuideLayer::onEnter()
{
	CCLayer::onEnter();
}

void CGuideLayer::onExit()
{
	CCLayer::onExit();
}

//设置剪裁区域
void CGuideLayer::SetSencil( float nRadius,float nX,float nY )
{
	static ccColor4F red = {1,0,0,1};

	const int nCount = 200;

	static CCPoint pt[nCount];

	const float angle = 2.0 * (float)M_PI/nCount;


	for (int i = 0; i < nCount; ++i)
	{
		float radian = i*angle;

		//x，y
		pt[i].x = nRadius * cosf(radian);
		pt[i].y = nRadius * sinf(radian);
	}

	//绘制
	CCDrawNode *pNode = CCDrawNode::create();
	pNode->drawPolygon(pt,nCount,red,0,red);		//绘制多边形

	//绘制一个一直放大和缩小的光效
	pNode->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(CCScaleBy::create(0.05f,0.95f),CCScaleTo::create(0.125f,1.0f))));

	pNode->setPosition(ccp(nX,nY));
	//设置剪裁模板
	if (m_pCClippingNode)
		m_pCClippingNode->setStencil(pNode);
}

bool CGuideLayer::init()
{
	if (CCLayer::init())
	{
		//添加一个层
		CCSize size= CCDirector::sharedDirector()->getWinSize();

		CCSprite * pSprite = CCSprite::create("iphone/HelloWorld.png");
		pSprite->setPosition(ccp(size.width/2,size.height/2));
		this->addChild(pSprite);

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create("iphone/CloseNormal.png", "iphone/CloseSelected.png", this, menu_selector(CGuideLayer::closeCallback) );
		CCMenu* pMenu =CCMenu::create(pCloseItem, NULL);

		pMenu->setPosition( ccp(size.width/2,size.height/2) );

		this->addChild(pMenu,1);


		m_pCClippingNode = CCClippingNode::create();
		m_pCClippingNode->setInverted(true);			//反向，
		this->addChild(m_pCClippingNode,10);



		CCLayerColor *pColorLayer = CCLayerColor::create(ccc4(0,0,0,110),size.width,size.height);

		m_pCClippingNode->addChild(pColorLayer);

		SetSencil(55.0f,size.width/2,size.height/2);

		return true;
	}

	return false;
}

void CGuideLayer::closeCallback( CCObject * pSender )
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//
//#else
//	CCDirector::sharedDirector()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
//#endif
	SetSencil(55.0f,100,100);
}
