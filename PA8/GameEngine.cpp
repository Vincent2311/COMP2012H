#include "GameEngine.h"

using namespace std;

// Please do not change the following already implemented code
void GameEngine::print_active_monsters()
{
    int i = 0;
    cout << "Active Monsters:" << endl;
    MonsterNode *cur = head;
    while (cur != nullptr)
    {
        cout << cur->data->get_display_str();
        cout << "@(" << cur->data->get_x();
        cout << "," << cur->data->get_y() << "), ";
        cur = cur->next;
        i++;
        if (i == 4)
        {
            i = 0;
            cout << endl;
        }
    }
}

bool GameEngine::player_win() const
{
    int px, py;
    player->get_position(px, py);
    if (px == MAP_SIZE - 1 && py == MAP_SIZE - 1)
    {
        for (int i = 0; i < MAP_SIZE; i++)
        {
            for (int j = 0; j < MAP_SIZE; j++)
            {
                if (map[i][j]->get_unit_type() == UnitType::MONSTER && map[i][j]->is_valid())
                {
                    return false;
                }
            }
        }
        return true;
    }
    else
        return false;
}

GameEngine::GameEngine()
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (i > 8 && i < 12 && j > 2 && j < 6)
                map[i][j] = new Wall(i, j);
            else if (i > 14 && i < 18 && j > 2 && j < 6)
                map[i][j] = new Grass(i, j);
            else if (i > 2 && i < 6 && j == 9)
                map[i][j] = new Gem(i, j, GemType::RUBY);
            else if (i > 2 && i < 6 && j == 10)
                map[i][j] = new Gem(i, j, GemType::EMERALD);
            else if (i > 2 && i < 6 && j == 11)
                map[i][j] = new Gem(i, j, GemType::SAPPHIRE);
            else if (i > 8 && i < 12 && j > 8 && j < 12)
                map[i][j] = new Book(i, j);
            else if (i > 14 && i < 18 && j > 8 && j < 12)
                map[i][j] = new Medicine(i, j);
            else if (i > 2 && i < 6 && j > 14 && j < 18)
                map[i][j] = new Zombie(i, j);
            else if (i > 8 && i < 12 && j > 14 && j < 18)
                map[i][j] = new Warlock(i, j);
            else if (i > 14 && i < 18 && j > 14 && j < 18)
                map[i][j] = new ShadowPriest(i, j);
            else
                map[i][j] = new EmptyLand(i, j);
        }
    }
}

char GameEngine::input()
{
    char c;
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.clear();
    return c;
}

void GameEngine::run()
{
    string name;
    cout << "Please enter your name:" << endl;
    cin >> name;
    cout << "Please choose your role:" << endl;
    cout << "1 Warrior" << endl;
    cout << "2 Archer" << endl;
    cout << "3 Mage" << endl;

    int x = 0, y = 0;
    char role = input();
    switch (role)
    {
    case '1':
        create_player(x, y, name, Role::WARRIOR);
        break;
    case '2':
        create_player(x, y, name, Role::ARCHER);
        break;
    case '3':
        create_player(x, y, name, Role::MAGE);
        break;
    default:
        exit(0);
        break;
    }
    ui.refresh(this->map, *player);

    while (true)
    {
        switch (input())
        {
        case 'w':
            this->player_move(Direction::UP);
            break;
        case 'a':
            this->player_move(Direction::LEFT);
            break;
        case 's':
            this->player_move(Direction::DOWN);
            break;
        case 'd':
            this->player_move(Direction::RIGHT);
            break;
        case 'h':
            this->player_attack();
            break;
        case 'j':
            this->player_skill();
            break;
        default:
            break;
        }
        if (player_win())
        {
            cout << endl;
            cout << "You win." << endl;
            break;
        }
        player_discover();
        activate_monsters();
        monster_follow();
        monster_act();
        ui.refresh(this->map, *player);
        print_active_monsters();
        if (!player->is_alive())
        {
            cout << endl;
            cout << "You lose." << endl;
            break;
        }
    }
}

// write your code here
GameEngine::~GameEngine()
{
    MonsterNode *cur = head;
    MonsterNode *nxt;
    while (cur)
    {
        nxt = cur->next;
        delete cur;
        cur = nxt;
    }

    delete player;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            delete map[i][j];
        }
    }
}

void GameEngine::create_player(int x, int y, const string &name, Role role)
{
    if (role == Role::ARCHER)
    {
        player = new Archer(x, y, name);
    }
    else if (role == Role::MAGE)
    {
        player = new Mage(x, y, name);
    }
    else
    {
        player = new Warrior(x, y, name);
    }
}

void GameEngine::activate_monster(Monster *monster)
{
    if (monster->is_valid() && !monster->is_active())
    {
        monster->set_active();
        MonsterNode *new_node = new MonsterNode;
        new_node->data = monster;
        new_node->next = head;
        head = new_node;
    }
}

void GameEngine::activate_monsters()
{
    int player_x = 0;
    int player_y = 0;
    player->get_position(player_x, player_y);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 1 && j == 1)
                continue;
            if (player_x - 1 + i < 0 || player_x - 1 + i >= MAP_SIZE || player_y - 1 + j < 0 || player_y - 1 + j >= MAP_SIZE) continue;
            if (map[player_x - 1 + i][player_y - 1 + j]->get_unit_type() == UnitType::MONSTER)
            {
                activate_monster(static_cast<Monster *>(map[player_x - 1 + i][player_y - 1 + j]));
            }
        }
    }
}

void GameEngine::deactivate_monster(Monster const *const monster)
{   
    if(!head) return;
    MonsterNode *cur = head;
    MonsterNode *nxt = head->next;
    if (cur->data == monster)
    {
        delete cur;
        head = nxt;
        return;
    }
    while (nxt)
    {
        if (nxt->data == monster)
        {
            cur->next = nxt->next;
            delete nxt;
            return;
        }
        cur = nxt;
        nxt = nxt->next;
    }
}

void GameEngine::player_move(Direction d)
{
    int player_x = 0;
    int player_y = 0;
    player->get_position(player_x, player_y);
    switch (d)
    {
    case Direction::UP:
        if (player_y != MAP_SIZE - 1)
        {
            if (!map[player_x][player_y + 1]->is_blocked())
            {
                player->move(d, 1);
                map[player_x][player_y + 1]->triggered_by(*player);
            }
            else
                player->move(d, 0);
        }
        else
            player->move(d, 0);
        break;
    case Direction::DOWN:
        if (player_y != 0)
        {
            if (!map[player_x][player_y - 1]->is_blocked())
            {
                player->move(d, 1);
                map[player_x][player_y - 1]->triggered_by(*player);
            }
            else
                player->move(d, 0);
        }
        else
            player->move(d, 0);
        break;
    case Direction::RIGHT:
        if (player_x != MAP_SIZE - 1)
        {
            if (!map[player_x + 1][player_y]->is_blocked())
            {
                player->move(d, 1);
                map[player_x + 1][player_y]->triggered_by(*player);
            }
            else
                player->move(d, 0);
        }
        else
            player->move(d, 0);
        break;
    case Direction::LEFT:
        if (player_x != 0)
        {
            if (!map[player_x - 1][player_y]->is_blocked())
            {
                player->move(d, 1);
                map[player_x - 1][player_y]->triggered_by(*player);
            }
            else
                player->move(d, 0);
        }
        else
            player->move(d, 0);
        break;
    default:
        break;
    }
}

void GameEngine::player_discover()
{
    int player_x = 0;
    int player_y = 0;
    player->get_position(player_x, player_y);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (i == 2 && j == 2)
                continue;
            if (player_x - 2 + i < 0||player_x - 2 + i >= MAP_SIZE || player_y - 2 + j < 0 || player_y - 2 + j >= MAP_SIZE) continue;
            map[player_x - 2 + i][player_y - 2 + j]->set_discovered();
        }
    }
}

void GameEngine::player_attack()
{
    int player_x = 0;
    int player_y = 0;
    player->get_position(player_x, player_y);
    int range = player->get_range();
    MapUnit *cur_unit;
    int atk = 0;
    int def = 0;
    player->get_atk_def(atk, def);
    for (int i = 1; i <= range; i++)
    {
        switch (player->get_direction())
        {
        case Direction::UP:
            if (player_y + i >= MAP_SIZE)
                return;
            cur_unit = map[player_x][player_y + i];
            break;
        case Direction::DOWN:
            if (player_y - i < 0)
                return;
            cur_unit = map[player_x][player_y - i];
            break;
        case Direction::LEFT:
            if (player_x - i < 0)
                return;
            cur_unit = map[player_x - i][player_y];
            break;
        case Direction::RIGHT:
            if (player_x + i >= MAP_SIZE)
                return;
            cur_unit = map[player_x + i][player_y];
            break;
        default:
            break;
        }
        int defeated = cur_unit->attacked_by(atk);
        if (cur_unit->get_unit_type() == UnitType::MONSTER)
        {
            if (defeated)
            {
                deactivate_monster(static_cast<Monster *>(cur_unit));
                player->gain_exp(1);
            }
            else
            {
                if (!static_cast<Monster *>(cur_unit)->is_active())
                {
                    static_cast<Monster *>(cur_unit)->set_active();
                    activate_monster(static_cast<Monster *>(cur_unit));
                }
            }
        }
    }
}

void GameEngine::player_skill()
{
    int atk = 0;
    int range = 0;
    int def = 0;
    player->get_atk_def(atk, def);
    int player_x = 0;
    int player_y = 0;
    player->get_position(player_x, player_y);
    MapUnit *cur_unit;
    if (player->skill(atk, range) == 1)
        return;
    else
    {
        for (int i = 1; i <= range; i++)
        {
            switch (player->get_direction())
            {
            case Direction::UP:
                if (player_y + i >= MAP_SIZE)
                    return;
                cur_unit = map[player_x][player_y + i];
                break;
            case Direction::DOWN:
                if (player_y - i < 0)
                    return;
                cur_unit = map[player_x][player_y - i];
                break;
            case Direction::LEFT:
                if (player_x - i < 0)
                    return;
                cur_unit = map[player_x - i][player_y];
                break;
            case Direction::RIGHT:
                if (player_x + i >= MAP_SIZE)
                    return;
                cur_unit = map[player_x + i][player_y];
                break;
            default:
                break;
            }
            int defeated = cur_unit->attacked_by(atk);
            if (cur_unit->get_unit_type() == UnitType::MONSTER)
            {
                if (defeated)
                {
                    deactivate_monster(static_cast<Monster *>(cur_unit));
                    player->gain_exp(1);
                }
                else
                {
                    if (!static_cast<Monster *>(cur_unit)->is_active())
                    {
                        static_cast<Monster *>(cur_unit)->set_active();
                        activate_monster(static_cast<Monster *>(cur_unit));
                    }
                }
            }
        }
    }
}

void GameEngine::monster_follow() {
    MonsterNode *cur = head;
    while(cur) {
        cur->data->follow(*player,map);
        cur = cur->next;
    }
}

void GameEngine::monster_act() {
    MonsterNode *cur = head;
    while(cur) {
        cur->data->action(*player,map);
        cur = cur->next;
    }
}