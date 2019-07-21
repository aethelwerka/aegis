load "keys"

KeyQuit = function()  
	if context:KeyDown(Keys.Q) or context:KeyDown(Keys.X) then
		return true
	end
end