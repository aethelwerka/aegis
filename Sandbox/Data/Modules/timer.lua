function newTimer(d)
    local timer = {
	
	delta = 0;
	delay = d;
	elapsed = 0;

	start = function(self)
		self.delta = os.clock()
	end;

	update = function(self)
		if (self.elapsed - self.delta > self.delay) then
			self.delta = os.clock()
			return true
		end
		self.elapsed = os.clock()
	end;
	}
	
    return timer
end

function Delay(n)
  os.execute("timeout " ..tonumber(n))
end

function sleep(sec)
  local t = os.clock()
  while os.clock() - t <= sec do
	context:Running()
	if KeyQuit() then break end
	if context:KeyDown(Keys.Space) then break end
  end
end

