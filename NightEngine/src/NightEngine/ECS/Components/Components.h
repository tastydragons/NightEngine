#pragma once

#include "Transform.h"
#include "RigidBody.h"
#include "WinData.h"
#include "Collider.h"
#include "NameTag.h"
#include "Sprite.h"
#include "Logic.h"
#include "Tag.h"
#include "Audio.h"
#include "HUDComponent.h"
#include "Attributes.h"
#include "PlayerAttributes.h"
#include "EnemyAttributes.h"

//Macros for creating default values of various components
#define C_TransformComponent Night::Transform::Transform()
#define C_RigidBodyComponent Night::RigidBody::RigidBody()
#define C_ColliderComponent Night::Collider::Collider()
#define C_NameTagComponent Night::NameTag::NameTag()
#define C_SpriteComponent Night::Sprite::Sprite()
#define C_LogicComponent Night::Logic::Logic()
#define C_TagComponent Night::Tag::Tag()
#define C_AudioComponent Night::Audio::Audio()
#define C_HUDComponent Night::HUDComponent::HUDComponent()
#define C_AttributesComponent Night::Attributes::Attributes()
#define C_PlayerAttributesComponent Night::PlayerAttributes::PlayerAttributes()
#define C_EnemyAttributesComponent Night::EnemyAttributes::EnemyAttributes()