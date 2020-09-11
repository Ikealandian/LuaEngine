local idx = 0
local running = true

function wait(seconds)
    wait_time = os.time() + seconds
    while(os.time() < wait_time) do
    end
end

while running do

    print("idx: ", idx)
    idx = idx + 1

    if idx > 10 then
        running = false
    end

    wait(1);

end