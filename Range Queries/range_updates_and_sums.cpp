/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <bits/stdc++.h>
using namespace std;

#define ll long long
vector<ll> seg;
vector<ll> lazyAdd;
vector<ll> lazySet;
vector<int> arr;

// lazy add and lazy set are just tags, we are updating the sum acc to that tag,
//  they are like markers, that below this, the sum is stale, and if we want to go deeper
//  we will need to update the sum and pass the tag down, and then reset the tag of the current node
void build(int index, int low, int high)
{
    if (low == high)
    {
        seg[index] = arr[low];
        return;
    }
    int mid = low + (high - low) / 2;
    build(2 * index + 1, low, mid);
    build(2 * index + 2, mid + 1, high);
    seg[index] = seg[2 * index + 1] + seg[2 * index + 2];
}

void applyAdd(int index, ll val, int len)
{
    seg[index] += val * len;
    lazyAdd[index] += val;
}

void applySet(int index, ll val, int len)
{
    seg[index] = val * len;
    lazySet[index] = val;
    lazyAdd[index] = 0; // set wipes pending add
}

void pushDown(int index, int low, int high)
{
    int mid = low + (high - low) / 2;
    int lLen = mid - low + 1;
    int rLen = high - mid;

    // first push down set then add then reset the cur node
    if (lazySet[index] != -1)
    {
        applySet(2 * index + 1, lazySet[index], lLen);
        applySet(2 * index + 2, lazySet[index], rLen);
        applyAdd(2 * index + 1, lazyAdd[index], lLen);
        applyAdd(2 * index + 2, lazyAdd[index], rLen);
        lazySet[index] = -1;
        lazyAdd[index] = 0;
    }
    else if (lazyAdd[index] != 0)
    {
        applyAdd(2 * index + 1, lazyAdd[index], lLen);
        applyAdd(2 * index + 2, lazyAdd[index], rLen);
        lazyAdd[index] = 0;
    }
}

ll rangeQuery(int index, int low, int high, int lRange, int hRange)
{
    if (hRange < low || high < lRange)
    {
        return 0ll;
    }
    if (lRange <= low && high <= hRange)
    {
        return seg[index];
    }
    pushDown(index, low, high);
    int mid = low + (high - low) / 2;
    ll lSum = rangeQuery(2 * index + 1, low, mid, lRange, hRange);
    ll rSum = rangeQuery(2 * index + 2, mid + 1, high, lRange, hRange);
    return lSum + rSum;
}

void rangeAdd(int index, int low, int high, int lRange, int hRange, ll val)
{
    if (hRange < low || high < lRange)
    {
        return;
    }
    if (lRange <= low && high <= hRange)
    {
        return applyAdd(index, val, high - low + 1);
    }
    pushDown(index, low, high);
    int mid = low + (high - low) / 2;
    rangeAdd(2 * index + 1, low, mid, lRange, hRange, val);
    rangeAdd(2 * index + 2, mid + 1, high, lRange, hRange, val);
    seg[index] = seg[2 * index + 1] + seg[2 * index + 2];
}

void rangeSet(int index, int low, int high, int lRange, int hRange, ll val)
{
    if (hRange < low || high < lRange)
    {
        return;
    }
    if (lRange <= low && high <= hRange)
    {
        return applySet(index, val, high - low + 1);
    }
    pushDown(index, low, high);
    int mid = low + (high - low) / 2;
    rangeSet(2 * index + 1, low, mid, lRange, hRange, val);
    rangeSet(2 * index + 2, mid + 1, high, lRange, hRange, val);
    seg[index] = seg[2 * index + 1] + seg[2 * index + 2];
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    arr.resize(n);
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    seg.resize(4 * n + 1, 0ll);
    lazyAdd.resize(4 * n + 1, 0ll);
    lazySet.resize(4 * n + 1, -1ll);
    build(0, 0, n - 1);

    vector<ll> ans;
    for (int i = 0; i < q; i++)
    {
        int type;
        cin >> type;
        if (type == 1 || type == 2)
        {
            int a, b;
            cin >> a >> b;
            a--;
            b--;
            ll val;
            cin >> val;
            if (type == 1)
            {
                rangeAdd(0, 0, n - 1, a, b, val);
            }
            else
            {
                rangeSet(0, 0, n - 1, a, b, val);
            }
        }
        else
        {
            int a, b;
            cin >> a >> b;
            a--;
            b--;
            ans.push_back(rangeQuery(0, 0, n - 1, a, b));
        }
    }

    for (auto &i : ans)
    {
        cout << i << endl;
    }

    return 0;
}