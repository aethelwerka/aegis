guard = {
    name="Guard",
	maxHp = 10,
	hp = 10,
	currentLine = 1,
	lines = {
		[1] = "Citizen.",
		[2] = "Disrespect the law, and you disrespect me.",
		[3] = "What do you need?",
		[4] = "Trouble?",
		[5] = "What is it?",
		[6] = "Staying safe I hope.",
		[7] = "Everything all right?",
		[8] = "Could sure use a warm bed right about now...",
		[9] = "Watch the skies, traveler.",
		[10] = "Bit late to be wandering around, isn't it?",
		[11] = "Only burglars and vampires creep around after dark. So which are you?",
		[12] = "You hear that? I swear, there's something out there. In the dark.",
		[13] = "The gods gave you two hands, and you use them both for your weapon. I can respect that."
	}
}

function guard:interact(actor)
	if (self.currentLine > 13) then self.currentLine = 1 end
	local talk = self.lines[self.currentLine]
	self:nextLine()
	return talk
end

function guard:nextLine()
	self.currentLine = self.currentLine + 1
end