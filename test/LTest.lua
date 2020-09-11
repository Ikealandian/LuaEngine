local idx = 0
local running = true

while running do

    print("idx: ", idx)
    idx = idx + 1

    if idx > 300 then
        running = false
    end

end