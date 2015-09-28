local module = {}


function module.new()
    return { left = 0, right = -1 }
end


function module.pushfront(list,value)
    local left = list.left - 1
    list.left = left
    list[left] = value
end

function module.pushback(list,value)
    local right = list.right + 1
    list.right = right
    list[right] = value
end


function module.popfront(list)
    local left = list.left
    if left > list.right then error("list is empty") end
    local value = list[left]
    list[left] = nil        -- to allow garbage collection
    list.left = left + 1
    return value
end

function module.popback(list)
    local right = list.right
    if list.left > right then error("list is empty") end
    local value = list[right]
    list[right] = nil        -- to allow garbage collection
    list.right = right - 1
    return value
end


return module
