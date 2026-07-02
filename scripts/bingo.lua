function on_app_started()
    print("Bingo app started - Ivo esteve aqui. Scripting")
end

function on_draw_started()
    print("Draw started")
end

function on_numbers_drawn(numbers)
    print("Numbers drawn: " .. table.concat(numbers, " - "))
end

function on_ball_animation_started(index, number)
    print("Ball " .. index .. " started: " .. number)
end

function on_ball_landed(index, number)
    print("Ball " .. index .. " landed: " .. number)
end

function on_draw_completed(numbers)
    print("Draw completed: " .. table.concat(numbers, " - "))
end