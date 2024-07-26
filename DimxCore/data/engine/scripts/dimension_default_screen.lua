ui:registerEvent('exit-dimension.onClick', "onExitDimensionClicked")
ui:registerEvent('enter-location.onClick', "onEnterLocationClicked")
ui:registerEvent('exit-location.onClick', "onExitLocationClicked")

g_locationsScreen = ui:find("locations-screen")
g_locationsList = ui:findListBox("locations-list")
g_locationsItems = nil

function onFocus(active)
    if g_locationsScreen:visible() then
        g_locationsScreen:setVisible(false)
    else
        g_locationsScreen:setVisible(true)
        g_locationsItems = dim:locations()
        g_locationsList:clearItems()
        for i = 1, #g_locationsItems do
            g_locationsList:addItem(g_locationsItems[i].name)
        end
    end
end

function onExitDimensionClicked()
    dim:exit()
end

function onEnterLocationClicked()
    local index = g_locationsList:selectedItem()

    if index <= 0 then
        return
    end

    if index > #g_locationsItems then
        env:loge("Invalid enter location index: " .. tostring(index))
        return
    end

    dim:enterLocation(g_locationsItems[index].id)
    g_locationsScreen:setVisible(false)
end

function onExitLocationClicked()
    local index = g_locationsList:selectedItem()

    if index <= 0 then
        return
    end

    if index > #g_locationsItems then
        env:loge("Invalid exit location index: " .. tostring(index))
        return
    end

    dim:exitLocation(g_locationsItems[index].id)
    g_locationsScreen:setVisible(false)
end
