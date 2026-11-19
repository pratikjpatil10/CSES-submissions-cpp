/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <bits/stdc++.h>
using namespace std;

#define ll long long
map<int, int> mp;
vector<ll> segTree;

void updateSegTree(int index, int low, int high, int pos, ll val)
{
    if (pos < low || high < pos)
    {
        return;
    }
    if (low == high && low == pos)
    {
        segTree[index] = val;
        return;
    }

    int mid = low + (high - low) / 2;
    updateSegTree(2 * index + 1, low, mid, pos, val);
    updateSegTree(2 * index + 2, mid + 1, high, pos, val);
    segTree[index] = segTree[2 * index + 1] + segTree[2 * index + 2];
}

ll querySegTree(int index, int low, int high, int lRange, int hRange)
{
    if (hRange < low || high < lRange)
    {
        return 0ll;
    }
    if (lRange <= low && high <= hRange)
    {
        return segTree[index];
    }

    int mid = low + (high - low) / 2;
    ll lSum = querySegTree(2 * index + 1, low, mid, lRange, hRange);
    ll rSum = querySegTree(2 * index + 2, mid + 1, high, lRange, hRange);
    return lSum + rSum;
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }
    vector<vector<int>> queries(q, vector<int>(3, -1));
    for (int i = 0; i < q; i++)
    {
        cin >> queries[i][0] >> queries[i][1];
        queries[i][2] = i;
    }

    sort(queries.begin(), queries.end());
    segTree.resize(4 * n, 0ll);

    int curInd = n - 1;
    vector<vector<ll>> ans;
    for (int i = q - 1; i >= 0; i--)
    {
        int l = queries[i][0] - 1;
        int r = queries[i][1] - 1;

        while (curInd >= l)
        {
            if (mp.find(arr[curInd]) != mp.end())
            {
                int prevInd = mp[arr[curInd]];
                updateSegTree(0, 0, n - 1, prevInd, 0ll);
            }
            mp[arr[curInd]] = curInd;
            updateSegTree(0, 0, n - 1, curInd, 1ll);
            curInd--;
        }

        ll curAns = querySegTree(0, 0, n - 1, l, r);
        ans.push_back({queries[i][2] * 1ll, curAns});
    }

    sort(ans.begin(), ans.end());
    for (auto &i : ans)
    {
        cout << i[1] << endl;
    }

    return 0;
}