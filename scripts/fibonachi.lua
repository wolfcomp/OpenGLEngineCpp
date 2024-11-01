function fibonachi(n)
    local cur = 1
    local prev = 0
    for i = 1, n do
        cur, prev = cur + prev, cur
    end
    return cur
end
