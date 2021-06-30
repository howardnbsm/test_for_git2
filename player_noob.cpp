#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>


//jizzjizzjizzjizz
using namespace std;
struct Point {
    int x, y;
    Point() : Point(0, 0) {}
	Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const {
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const Point& rhs) const {
		return !operator==(rhs);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
};

array<Point, 8> directions{Point(-1, -1), Point(-1, 0), Point(-1, 1),
                            Point(0, -1), /*{0, 0}, */ Point(0, 1),
                            Point(1, -1), Point(1, 0), Point(1, 1)};

int player, cur_player;
const int SIZE = 8;
array<array<int, SIZE>, SIZE> board;
vector<Point> next_valid_spots;
int v;
int ansx, ansy;

//search where can put(from main)
int get_disc(Point p) 
{
    return board[p.x][p.y];
}

bool is_spot_on_board(Point p)
{
    return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
}

bool is_disc_at(Point p, int disc)
{
    if (!is_spot_on_board(p))
        return false;
    if (get_disc(p) != disc)
        return false;
    return true;
}

bool is_spot_valid(Point center)
{
    if (get_disc(center) != 0)
        return false;
    for (Point dir: directions) {
        // Move along the direction while testing.
        Point p = center + dir;
        if (!is_disc_at(p, 3-cur_player))
            continue;
        p = p + dir;
        while (is_spot_on_board(p) && get_disc(p) != 0) {
            if (is_disc_at(p, cur_player))
                return true;
            p = p + dir;
        }
    }
    return false;
}

vector<Point> get_valid_spots()
{
    vector<Point> valid_spots;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Point p = Point(i, j);
            if (board[i][j] != 0)
                continue;
            if (is_spot_valid(p))
                valid_spots.push_back(p);
        }
    }
    return valid_spots;
}

void change(int x,int y,bool play)
{
    int flagx, flagy;
    int now, change;

    if(player==1)
    {
        if(play)
        {
            now = 1;
            change = 2;
            board[x][y] = 1;
        }
        else
        {   
            now = 2;
            change = 1;
            board[x][y] = 2;
        }
    }
    else if(player==2)
    {
        if(play)
        {
            now = 2;
            change = 1;
            board[x][y] = 2;
        }
        else
        {   
            now = 1;
            change = 2;
            board[x][y] = 1;
        }
    }

    bool flag = false;
    //up
    for (int i = x; i >= 0; i--)
    {
        if(board[i][y]==0)
        {
            flag = false;
            break;
        }
        else if(board[i][y]==change)
        {
            flag = true;
        }
        else if(board[i][y]==now&&flag)
        {
            flagx = i;
            break;
        }
        else if(board[i][y]==now&&!flag && i!=x)
            break;
    }
    if(flag)
    {
        for (int i = x - 1; i > flagx;i--)
            board[i][y] = now;
    }
    flag = false;

    //down
    for (int i = x; i < 8;i++)
    {
        if(board[i][y]==0)
        {
            flag = false;
            break;
        }
        else if(board[i][y]==change)
        {
            flag = true;
        }
        else if(board[i][y]==now&&flag)
        {
            flagx = i;
            break;
        }
        else if(board[i][y]==now&&!flag && i!=x)
            break;
    }
    if(flag)
    {
        for (int i = x + 1; i < flagx;i++)
            board[i][y] = now;
    }
    flag = false;

    //right
    for (int i = y; i < 8;i++)
    {
        if(board[x][i]==0)
        {
            flag = false;
            break;
        }
        else if(board[x][i]==change)
        {
            flag = true;
        }
        else if(board[x][i]==now&&flag)
        {
            flagy = i;
            break;
        }
        else if(board[x][i]==now&&!flag && i!=y)
            break;
    }
    if(flag)
    {
        for (int i = y + 1; i < flagy;i++)
            board[x][i] = now;
    }
    flag = false;

    //left
    for (int i = y; i >= 0; i--)
    {
        if(board[x][i]==0)
        {
            flag = false;
            break;
        }
        else if(board[x][i]==change)
        {
            flag = true;
        }
        else if(board[x][i]==now&&flag)
        {
            flagy = i;
            break;
        }
        else if(board[x][i]==now&&!flag && i!=y)
            break;
    }
    if(flag)
    {
        for (int i = y - 1; i > flagy;i--)
            board[x][i] = now;
    }
    flag = false;

    //left up
    for (int j = x, k = y; j >= 0 && k >= 0;j--,k--)
    {
        if(board[j][k]==0)
        {
            flag=false;
            break;
        }
        else if(board[j][k]==change)
        {
            flag = true;
        }
        else if(board[j][k]==now&&flag)
        {
            flagx = j;
            flagy = k;
            break;
        }
        else if(board[j][k]==now&&!flag && j!=x && k!=y)
            break;
    }
    if(flag)
    {
        for (int j = x - 1, k = y - 1; j > flagx && k > flagy;j--,k--)
            board[j][k] = now;
    }
    flag = false;

    //right up
    for (int j = x, k = y; j >= 0 && k < 8;j--,k++)
    {
        if(board[j][k]==0)
        {
            flag=false;
            break;
        }
        else if(board[j][k]==change)
        {
            flag = true;
        }
        else if(board[j][k]==now&&flag)
        {
            flagx = j;
            flagy = k;
            break;
        }
        else if(board[j][k]==now&&!flag && j!=x && k!=y)
            break;
    }
    if(flag)
    {
        for (int j = x - 1, k = y + 1; j > flagx && k < flagy;j--,k++)
            board[j][k] = now;
    }
    flag = false;

    //left down
    for (int j = x, k = y; j < 8 && k >= 0;j++,k--)
    {
        if(board[j][k]==0)
        {
            flag=false;
            break;
        }
        else if(board[j][k]==change)
        {
            flag = true;
        }
        else if(board[j][k]==now&&flag)
        {
            flagx = j;
            flagy = k;
            break;
        }
        else if(board[j][k]==now&&!flag && j!=x && k!=y)
            break;
    }
    if(flag)
    {
        for (int j = x + 1, k = y - 1; j < flagx && k > flagy;j++,k--)
            board[j][k] = now;
    }
    flag = false;

    //right down
    for (int j = x, k = y; j < 8 && k < 8;j++,k++)
    {
        if(board[j][k]==0)
        {
            flag=false;
            break;
        }
        else if(board[j][k]==change)
        {
            flag = true;
        }
        else if(board[j][k]==now&&flag)
        {
            flagx = j;
            flagy = k;
            break;
        }
        else if(board[j][k]==now&&!flag && j!=x && k!=y)
            break;
    }
    if(flag)
    {
        for (int j = x + 1, k = y + 1; j < flagx && k < flagy;j++,k++)
            board[j][k] = now;
    }
    flag = false;
}

//估計值
int h()
{
    int ans = 0;
    for(int i=0;i<64;i++)
    {
        int j = i / 8;
        int k = i % 8;
        if(player==1)
        {
            if(board[j][k]==1)
                ans++;
            else if(board[j][k]==2)
                ans--;
        }
        else if(player==2)
        {
            if(board[j][k]==1)
                ans--;
            else if(board[j][k]==2)
                ans++;
        }
    }
    return ans;
}

int ab(int a,int b,int depth,bool maxplayer)
{
    if(maxplayer)
        cur_player = player;
    else
        cur_player = 3 - player;

    vector<Point> next_spots = get_valid_spots();
    
    if(depth==0||next_spots.empty())
	        return h();

    int tmp[8][8];
    if(maxplayer)
    {
        v = INT32_MIN;

        for (int i = 0; i < next_spots.size();i++)
        {
            for (int j = 0; j < 64;j++)
            {
                int m = j % 8;
                int n = j / 8;
                tmp[m][n] = board[m][n];
            }
			
            change(next_spots[i].x, next_spots[i].y, maxplayer);
            
            
            v = max(v, ab(a, b, depth - 1, false));
            for (int j = 0; j < 64;j++)
            {
                int m = j % 8;
                int n = j / 8;
                board[m][n] = tmp[m][n];
            }

            if(v>a)
            {
				if(depth==10)
                {
					
					ansx = next_spots[i].x;
                    ansy = next_spots[i].y;  
                }
            }
            a=max(a,v);
            if(a>=b)
                break;
                
        }
        return v;
    }
    else
    {
        v = INT32_MAX;

        for (int i = 0; i < next_spots.size();i++)
        {
            for (int j = 0; j < 64;j++)
            {
                int m = j % 8;
                int n = j / 8;
                tmp[m][n] = board[m][n];
            }

            change(next_spots[i].x, next_spots[i].y, maxplayer);
			            
            v = min(v, ab(a, b, depth - 1, true));

            for (int j = 0; j < 64;j++)
            {
                int m = j % 8;
                int n = j / 8;
                board[m][n] = tmp[m][n];
            }
            b = min(b, v);
            if(b<=a)
                break;
        }
        return v;
    }
}

void read_board(ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
        }
    }
}

void read_valid_spots(ifstream& fin) {
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({x, y});
    }
}

void write_valid_spot(std::ofstream& fout) {
    ab(INT32_MIN, INT32_MAX, 10, true);
    //fout << next_valid_spots[1].x << " " << next_valid_spots[1].y << endl;
    fout << ansx << " " << ansy << endl;
    fout.flush();
}

int main(int, char** argv) {
    ifstream fin(argv[1]);
    ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}