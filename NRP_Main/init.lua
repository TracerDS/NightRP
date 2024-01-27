local modules = getLoadedModules()

local initModuleMessage = 'NightRP gamemode (%s v%s - %s) loaded correctly!'

addEventHandler('onResourceStart', resourceRoot, function()
    if not modules then
        cancelEvent(true, 'Permissions not set up properly!')
        return
    end
    for _,module in ipairs(modules) do
        local info = getModuleInfo(module)
        if info.name == 'NRP Module' then
            if info.author ~= 'Tracer' then
                cancelEvent(true, 'Corrupted module!')
                return
            end
        end
        outputServerLog(initModuleMessage:format(info.name, info.version, info.author))
    end
end, false, 'high+9999')