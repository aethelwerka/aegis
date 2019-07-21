-- System Modules
load "keys"
load "colors"
load "events"
load "timer"
load "core"

-- Game Modules
load "hints"

screen_width = 950; screen_height = 540
context = Context.new("Aegis", screen_width, screen_height)
if (context == nil) then return end

context:SetBlendMode(Blend.Alpha)
context:SetClearColor(Color.Purple)

context:Clear()
context:Sync()

sleep(1)

text = TextRenderer.new(context)
if (text == nil) then return end

renderMessage=function(message, color)
	local pos = vec2.new(screen_width/2 - text:GetWidth(message, 0.5) / 2, 125)
	text:RenderText(message, pos, 0.5, color)
end

renderHint=function(locale, n)
	local hints_table = hints[locale]
	local hint = hints_table[n]
	renderMessage(hint, Color.Sand)
end

load "player.actor"
load "npc.guard"

output = guard.name .. ": " .. guard:interact(actor)

context:SetClearColor(Color.Black) 

colors = { Color.Red, Color.White, Color.Purple, Color.Yellow, Color.Black }

i = 1

timer = newTimer(0.5); -- half second
timer:start()

paused = false

while context:Running() do

	if paused == false then
		if timer:update() then
			context:SetClearColor(colors[i]) 
			i = i + 1
			if (i > length(colors)) then i = 1 end
		end

		if (context:KeyDown(Keys.N)) then
			output = guard.name .. ": " .. guard:interact(actor)
			sleep(0.2)
		end
	end
	
	if (context:KeyDown(Keys.P)) then
		if paused ~= false then
			paused = false
		else 
			paused = true
		end
		sleep(0.2)
	end
	
	context:Clear()

	renderMessage(output, colors[i])

	context:Sync()

	if KeyQuit() then break end
end