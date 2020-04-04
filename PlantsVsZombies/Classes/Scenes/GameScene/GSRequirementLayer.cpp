/**
 *Copyright (c) 2020 LZ.All Right Reserved
 *Author : LZ
 *Date: 2020.2.19
 *Email: 2117610943@qq.com
 */

#include "GSRequirementLayer.h"
#include "GSPauseQuitLayer.h"
#include "GameScene.h"

#include "Based/GameType.h"

int pressKeySpace = 0;

Layer* GSRequirementLayer::addLayer()
{
	return GSRequirementLayer::create();
}

bool GSRequirementLayer::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	GameScene::setPauseGame(true);
	showRequirement();
	keyboardControl();

	return true;
}

void GSRequirementLayer::showRequirement()
{
	_requirement = new UserWinRequirement(this);
	_requirement->createDialogBox(GameTypes::None);
	_requirement->setShowDialogAction();

	auto button = Button::create("Continue1.png", "Continue.png", "", TextureResType::PLIST);
	button->setTitleText(Global::getInstance()->userInformation->getGameText().find("ȷ��")->second);
	button->setTitleFontName(GAME_FONT_NAME_1);
	button->setTitleFontSize(30);
	button->setTitleColor(Color3B::YELLOW);
	button->setPosition(Vec2(_requirement->getDialog()->getContentSize().width / 2.0f, 10));
	button->setGlobalZOrder(10);
	button->setScale(0.5f);
	button->runAction(FadeIn::create(0.5f));
	button->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				AudioEngine::setVolume(AudioEngine::play2d(
					Global::getInstance()->userInformation->getMusicPath().find("gravebutton")->second), 
					Global::getInstance()->userInformation->getSoundEffectVolume());
				break;
			case ui::Widget::TouchEventType::ENDED:
				_requirement->setDelectDialogAction();
				this->runAction(Sequence::create(Spawn::create(DelayTime::create(0.2f), FadeOut::create(0.2f), nullptr),
					CallFunc::create([&]()
					{
						this->removeFromParent();
						GSPauseQuitLayer::resumeLayer();
						GameScene::setPauseGame(false);
					}), nullptr));
				break;
			}
		});
	_requirement->getDialog()->addChild(button);
}

void GSRequirementLayer::keyboardControl()
{
	auto KeyBoard = EventListenerKeyboard::create();
	KeyBoard->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event)
	{
		switch (code)
		{
		case cocos2d::EventKeyboard::KeyCode::KEY_SPACE:                     /* �ո���ָ� */
			KeyBoard->setEnabled(false);
			AudioEngine::setVolume(AudioEngine::play2d(
				Global::getInstance()->userInformation->getMusicPath().find("tap")->second), 
				Global::getInstance()->userInformation->getSoundEffectVolume());
			_requirement->setDelectDialogAction();
			this->runAction(Sequence::create(Spawn::create(DelayTime::create(0.2f), FadeOut::create(0.2f), nullptr),
				CallFunc::create([&, KeyBoard]()
				{
					this->removeFromParent();
					GSPauseQuitLayer::resumeLayer();
					GameScene::setPauseGame(false);
					KeyBoard->setEnabled(true);
				}), nullptr));
			break;
		default:
			break;
		}
	};
	_director->getEventDispatcher()->addEventListenerWithSceneGraphPriority(KeyBoard, this);
}
