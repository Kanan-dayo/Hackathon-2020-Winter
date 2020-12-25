#include "enemyMana.h"

CEnemyMana::CEnemyMana()
{
}

CEnemyMana::~CEnemyMana()
{
}

CEnemyMana * CEnemyMana::Create(void)
{
	CEnemyMana * pEnemyMana = new CEnemyMana;
	pEnemyMana->Init();
	return pEnemyMana;
}

void CEnemyMana::Init(void)
{
}

void CEnemyMana::Uninit(void)
{
}

void CEnemyMana::Update(void)
{
}

void CEnemyMana::Draw(void)
{
}
