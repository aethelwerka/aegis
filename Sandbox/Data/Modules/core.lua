-- Count Table Length
function length(tab)
	count = 0
	for k,v in pairs(tab) do
		 count = count + 1
	end
	return count
end
