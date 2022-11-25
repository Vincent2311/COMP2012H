#include "GameEngine.h"

using namespace std;

char GameEngine::input()
{
    char c;
    cin >> c;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.clear();
    return c;
}

void GameEngine::create_player(int x, int y, const string &name, Role role)
{
    switch (role)
    {
    case Role::WARRIOR:
        player = new Warrior(x, y, name);
        break;
    case Role::MAGE:
        player = new Mage(x, y, name);
        break;
    case Role::ARCHER:
        player = new Archer(x, y, name);
        break;
    default:
        break;
    }
}

void GameEngine::activate_monster(Monster *monster)
{
    if (!monster->is_valid() || monster->is_active())
    {
        return;
    }
    monster->set_active();
    MonsterNode *ptr = new MonsterNode;
    ptr->data = monster;
    ptr->next = head;
    head = ptr;
}
void GameEngine::activate_monsters()
{
    int px, py;
    player->get_position(px, py);
    // for (int i = px - 1; i <= px + 1; i++)
    for (int i = px + 1; i >= px - 1; i--)
    {
        if (i < 0 || i >= MAP_SIZE)
            continue;
        // for (int j = py - 1; j <= py + 1; j++)
        for (int j = py + 1; j >= py - 1; j--)
        {
            if (j < 0 || j >= MAP_SIZE)
                continue;
            if (map[i][j]->get_unit_type() == UnitType::MONSTER)
            {
                activate_monster((Monster *)map[i][j]);
            }
        }
    }
}
void GameEngine::deactivate_monster(Monster const *const monster)
{
    MonsterNode *prev = nullptr;
    MonsterNode *cur = head;
    while (cur != nullptr && cur->data != monster)
    {
        prev = cur;
        cur = cur->next;
    }
    if (cur != nullptr)
    {
        if (cur == head)
            head = head->next;
        else
            prev->next = cur->next;
        delete cur;
    }
}
void GameEngine::player_move(Direction d)
{
    int dstx, dsty;
    player->get_position(dstx, dsty);
    switch (d)
    {
    case Direction::UP:
        dsty += 1;
        break;
    case Direction::DOWN:
        dsty -= 1;
        break;
    case Direction::RIGHT:
        dstx += 1;
        break;
    case Direction::LEFT:
        dstx -= 1;
        break;
    default:
        break;
    }
    if (dstx < 0 || dstx >= MAP_SIZE || dsty < 0 || dsty >= MAP_SIZE || map[dstx][dsty]->is_blocked())
        player->move(d, 0);
    else
    {
        player->move(d, 1);
        map[dstx][dsty]->triggered_by(*player);
    }
}
void GameEngine::player_discover()
{
    int px, py;
    player->get_position(px, py);
    for (int i = px - 2; i <= px + 2; i++)
    {
        if (i < 0 || i >= MAP_SIZE)
            continue;
        for (int j = py - 2; j <= py + 2; j++)
        {
            if (j < 0 || j >= MAP_SIZE)
                continue;
            map[i][j]->set_discovered();
        }
    }
}
void GameEngine::player_attack()
{
    int atk, def, range;
    player->get_atk_def(atk, def);
    range = player->get_range();

    int px, py, dx, dy, nx, ny;
    player->get_position(px, py);
    switch (player->get_direction())
    {
    case Direction::UP:
        dx = 0, nx = 1;
        dy = 1, ny = range;
        break;
    case Direction::DOWN:
        dx = 0, nx = 1;
        dy = -1, ny = range;
        break;
    case Direction::RIGHT:
        dx = 1, nx = range;
        dy = 0, ny = 1;
        break;
    case Direction::LEFT:
        dx = -1, nx = range;
        dy = 0, ny = 1;
        break;
    default:
        break;
    }
    for (int i = nx; i > 0; i--)
    {
        px += dx;
        if (px < 0 || px >= MAP_SIZE)
            continue;
        for (int j = ny; j > 0; j--)
        {
            py += dy;
            if (py < 0 || py >= MAP_SIZE)
                continue;
            map[px][py]->set_discovered();
            int result = map[px][py]->attacked_by(atk);
            if (map[px][py]->get_unit_type() == UnitType::MONSTER)
            {
                Monster *monster_ptr = (Monster *)map[px][py];
                if (result > 0)
                {
                    deactivate_monster(monster_ptr);
                    player->gain_exp(1);
                }
                else if (!monster_ptr->is_active())
                {
                    activate_monster(monster_ptr);
                }
            }
        }
    }
}
void GameEngine::player_skill()
{
    int atk, range;
    int result = player->skill(atk, range);
    if (result > 0)
        return;

    int px, py, dx, dy, nx, ny;
    player->get_position(px, py);
    switch (player->get_direction())
    {
    case Direction::UP:
        dx = 0, nx = 1;
        dy = 1, ny = range;
        break;
    case Direction::DOWN:
        dx = 0, nx = 1;
        dy = -1, ny = range;
        break;
    case Direction::RIGHT:
        dx = 1, nx = range;
        dy = 0, ny = 1;
        break;
    case Direction::LEFT:
        dx = -1, nx = range;
        dy = 0, ny = 1;
        break;
    default:
        break;
    }
    for (int i = nx; i > 0; i--)
    {
        px += dx;
        if (px < 0 || px >= MAP_SIZE)
            continue;
        for (int j = ny; j > 0; j--)
        {
            py += dy;
            if (py < 0 || py >= MAP_SIZE)
                continue;
            map[px][py]->set_discovered();
            int result = map[px][py]->attacked_by(atk);
            if (map[px][py]->get_unit_type() == UnitType::MONSTER)
            {
                Monster *monster_ptr = (Monster *)map[px][py];
                if (result > 0)
                {
                    deactivate_monster(monster_ptr);
                    player->gain_exp(1);
                }
                else if (!monster_ptr->is_active())
                {
                    activate_monster(monster_ptr);
                }
            }
        }
    }
}
void GameEngine::monster_follow()
{
    MonsterNode *cur = head;
    while (cur != nullptr)
    {
        cur->data->follow(*player, map);
        cur = cur->next;
    }
}
void GameEngine::monster_act()
{
    MonsterNode *cur = head;
    while (cur != nullptr)
    {
        cur->data->action(*player, map);
        cur = cur->next;
    }
}
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
GameEngine::~GameEngine()
{
    MonsterNode *cur = head;
    while (head != nullptr)
    {
        head = head->next;
        delete cur;
        cur = head;
    }
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            delete map[i][j];
        }
    }
    delete player;
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