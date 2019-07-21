healer = {
    name = "Lucia",
    interact=function(healer, player)
        if(player.hp == player.maxHp) then
            healer:say("Come back when you're wounded.")
        else
            healer:say("Hey. Let me heal you.")
            player.hp = player.maxHp
        end
    end
}