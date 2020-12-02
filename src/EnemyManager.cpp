#include "../include/EnemyManager.hpp"

std::vector<Enemy> EnemyManager::enemies;
Json::Value EnemyManager::data;

void EnemyManager::Init()
{
    std::ifstream file("data/levels.json");
    if (!file.is_open())
    {
        printf("Some data files is missing!");
    }
    file >> data;
    file.close();
}

void EnemyManager::AddEnemy(std::string type)
{
    enemies.push_back(Enemy(data[type]));
}

void EnemyManager::Update()
{
    for (size_t i = 0; i < enemies.size(); ++i)
    {
        enemies.at(i).Update();
    }


}

bool EnemyManager::isEnemies()
{
    return !enemies.empty();
}



Enemy::Enemy(Json::Value data)
{
    std::ifstream file("data/levels.json");
    if (!file.is_open())
    {
        printf("Some data files is missing!");
    }
    file >> data;
    file.close();
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{

}

