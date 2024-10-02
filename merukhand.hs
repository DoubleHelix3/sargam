-- 'S','r','R','g',...
type Note = Char

-- assuming input doesn't have any duplicates 
merukhand :: [Note] -> [[Note]]
merukhand [note] = [[note]]
merukhand notes = concat $ map permutationsEndingWith $ reverse notes
  where permutationsEndingWith note = map (++ [note]) $ merukhand (filter (/=note) notes)
