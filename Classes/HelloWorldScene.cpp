#include "HelloWorldScene.h"
#include "plistTool.h"
#include "CCFileUtilsWin32.h"

#ifdef WIN32
#include <io.h>
#include <direct.h>
#include <Windows.h>
#endif


USING_NS_CC;

void filesearch(std::string path,std::vector<std::string> &fileVecotr)
{
	struct _finddata_t filefind;
	if(path[path.size()-1]=='\\')
		path.resize(path.size()-1);

	std::string curr=path;
	int done=0,handle;
	if((handle=_findfirst(curr.c_str(),&filefind))==-1)
		return;
	
	//curr=path+"\\"+filefind.name;
	fileVecotr.push_back(filefind.name);
	
	while(!(done=_findnext(handle,&filefind)))
	{
		if(!strcmp(filefind.name,".."))
			continue;

		if (filefind.attrib == _A_SUBDIR)
			continue;

		fileVecotr.push_back(filefind.name);
	}   
	_findclose(handle);     
}


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HANDLE hHandle = NULL;

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    


    return true;
}

DWORD WINAPI ThreadLoad(
	LPVOID lpThreadParameter
	)
{
#ifdef WIN32

	std::string resourceName = ((CCFileUtilsWin32*)CCFileUtils::sharedFileUtils())->getResourcePath();
	std::string inputfilename = resourceName.append("plist\\*.plist");

	std::vector<std::string> vVecotr;

	filesearch(inputfilename,vVecotr);

	if (vVecotr.size() == 0)
	{
		CCMessageBox("please put plist file into plist filefolder", "warnning");
		return 0;
	}
	else
	{
		for (int i = 0; i < vVecotr.size();++i)
		{
			std::string path = "plist";

			path.append("/");

			path.append(vVecotr[i]);

			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path.c_str());
		}
	}

	CCDictionary *pDict = CCSpriteFrameCache::sharedSpriteFrameCache()->GetSpriteFrameDict();
	if (pDict)
	{
		CCDictElement* pElement = NULL;
		CCDICT_FOREACH(pDict, pElement)
		{
			std::string spriteFrameName = pElement->getStrKey();
			CCSpriteFrame *pSpriteFrame = dynamic_cast<CCSpriteFrame*>(pElement->getObject());
			if (pSpriteFrame == NULL)
			{
				continue;
			}

			//解析spriteFrameName
			//不是图片
			if (spriteFrameName.find_last_of(".png") == std::string::npos)
				continue;

			//看看有没有前缀路径

			std::string path = spriteFrameName;
			size_t startPos = spriteFrameName.find_last_of("//"); 
			if (startPos != std::string::npos)
			{
				std::vector<std::string> cVector;

				//结尾
				path = path.erase(startPos);

				char *pToken = strtok((char*)path.c_str(),"//");

				while(pToken)
				{
					std::string fullpath = "";

					for (int i = 0;i < cVector.size();++i)
					{
						fullpath.append(cVector[i]);
						fullpath.append("//");
					}

					fullpath.append(pToken);

					cVector.push_back(pToken);


					if (_access(fullpath.c_str(),0) == -1)
					{
						mkdir(fullpath.c_str());
					}

					pToken = strtok(NULL,"//");
				}

			}

			//获取图片原始大小
			CCSize size = pSpriteFrame->getOriginalSize();

			//获取图片渲染
			CCSprite* buttonBgSprite = CCSprite::createWithSpriteFrame(pSpriteFrame);



			CCRenderTexture *pRt = CPlistTool::sharePlistToolManager()->getRenderTexture(size.width,size.height,buttonBgSprite,size.width/2,size.height/2);

			CPlistTool::sharePlistToolManager()->createPng(spriteFrameName.c_str(),pRt);

		}
	}


#endif

	return 0;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	//修改测试产生图片
	//获取帧框
	//pthread_create(&loadthread,HelloWorld::ThreadLoad,NULL,NULL);

	//if (hHandle)
	//{
	//	WaitForSingleObject(hHandle,INFINITE);
	//	hHandle = NULL;
	//}
	//
	//DWORD dwThread;
	//hHandle = CreateThread(NULL,0,ThreadLoad,0,0,&dwThread);

#ifdef WIN32

	std::string resourceName = ((CCFileUtilsWin32*)CCFileUtils::sharedFileUtils())->getResourcePath();
	std::string inputfilename = resourceName.append("plist\\*.plist");

	std::vector<std::string> vVecotr;

	filesearch(inputfilename,vVecotr);

	if (vVecotr.size() == 0)
	{
		CCMessageBox("please put plist file into plist filefolder", "warnning");
		return;
	}
	else
	{
		for (int i = 0; i < vVecotr.size();++i)
		{
			std::string path = "plist";

			path.append("/");

			path.append(vVecotr[i]);

			CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(path.c_str());
		}
	}

	CCDictionary *pDict = CCSpriteFrameCache::sharedSpriteFrameCache()->GetSpriteFrameDict();
	if (pDict)
	{
		CCDictElement* pElement = NULL;
		CCDICT_FOREACH(pDict, pElement)
		{
			std::string spriteFrameName = pElement->getStrKey();
			CCSpriteFrame *pSpriteFrame = dynamic_cast<CCSpriteFrame*>(pElement->getObject());
			if (pSpriteFrame == NULL)
			{
				continue;
			}

			//解析spriteFrameName
			//不是图片
			if (spriteFrameName.find_last_of(".png") == std::string::npos)
				continue;

			//看看有没有前缀路径

			std::string path = spriteFrameName;
			size_t startPos = spriteFrameName.find_last_of("//"); 
			if (startPos != std::string::npos)
			{
				std::vector<std::string> cVector;

				//结尾
				path = path.erase(startPos);

				char *pToken = strtok((char*)path.c_str(),"//");

				while(pToken)
				{
					std::string fullpath = "";

					for (int i = 0;i < cVector.size();++i)
					{
						fullpath.append(cVector[i]);
						fullpath.append("//");
					}

					fullpath.append(pToken);

					cVector.push_back(pToken);


					if (_access(fullpath.c_str(),0) == -1)
					{
						mkdir(fullpath.c_str());
					}

					pToken = strtok(NULL,"//");
				}

			}

			//获取图片原始大小
			CCSize size = pSpriteFrame->getOriginalSize();

			//获取图片渲染
			CCSprite* buttonBgSprite = CCSprite::createWithSpriteFrame(pSpriteFrame);



			CCRenderTexture *pRt = CPlistTool::sharePlistToolManager()->getRenderTexture(size.width,size.height,buttonBgSprite,size.width/2,size.height/2);

			CPlistTool::sharePlistToolManager()->createPng(spriteFrameName.c_str(),pRt);

		}
	}
#endif

}



HelloWorld::~HelloWorld()
{
#ifdef WIN32
	if (hHandle)
	{
		WaitForSingleObject(hHandle,INFINITE);
	}
#endif
}
