{-# LANGUAGE FlexibleInstances #-}
import Data.List

data Name = Sa 
  | KomalRe 
  | Re
  | KomalGa 
  | Ga 
  | Ma 
  | TeevraMa 
  | Pa 
  | KomalDha 
  | Dha 
  | KomalNi 
  | Ni
  deriving (Eq, Ord, Enum)

data Octave = Low | Middle | High
  deriving (Eq, Ord, Enum)

data Note = Note Octave Name
  deriving (Eq, Ord)

sa = Note Middle Sa
re = Note Middle Re
ga = Note Middle Ga
ma = Note Middle Ma
pa = Note Middle Pa
dha = Note Middle Dha
ni = Note Middle Ni

komal :: Note -> Note
komal (Note octave name) = case name of 
  Sa -> Note (pred octave) Ni
  _  -> Note octave (pred name)

teevra :: Note -> Note
teevra (Note octave name) = case name of 
  Ni -> Note (succ octave) Sa
  _  -> Note octave (succ name)

low :: Note -> Note
low (Note octave name) = Note (pred octave) name

high :: Note -> Note
high (Note octave name) = Note (succ octave) name

extend :: Direction -> [Note] -> [[Note]]
extend Decending notes = reverse (extend Accending notes)
extend Accending notes = [map f notes | f <- fs]
  where fs = [] ++ 
          (if all (>= high sa) notes then [low . low] else []) ++ 
          (if all (>= sa) notes then [low] else []) ++ 
          [id] ++
          (if all (<= high sa) notes then [high] else []) ++ 
          (if all (<= sa) notes then [high . high ] else [])


data Direction = Decending | Accending

data Raag = Yaman | Durga | Hamsadhavani | Jaunpuri

notes :: Raag -> Direction -> [Note]
notes raag direction = concat . (extend direction) $ case (raag, direction) of
  (Yaman, Accending) -> [sa, re, ga, teevra ma, pa, dha, ni]
  (Hamsadhavani, Accending) -> [sa, re, ga, pa, ni]
  (Durga, Accending) -> [sa, re, ma, pa, dha]
  (Jaunpuri, Accending) -> [sa, re, ma, pa, komal dha, komal ni]
  (Jaunpuri, Decending) -> [komal ni, komal dha, pa, ma, komal ga, re, sa]
  
  (raag, Decending) -> reverse (notes raag Accending)

chalan :: Raag -> Direction -> [[Note]]
chalan raag direction = concat . (map (extend direction)) $ case (raag, direction) of 
  (Yaman, Accending) -> [[low ni, re, ga], [teevra ma, dha, ni]]
  _ -> []

start :: Raag -> Note
start raag = case raag of 
  Yaman -> low ni
  _ -> sa


walk :: Raag -> Direction -> Note -> Int -> [Note]
walk _ _ note 1 = [note]
walk raag direction note k = path ++ rest
  where rest = drop 1 $ walk raag direction (last path) (k+1-(length path))
        path = head $ ((filter ((<=k).length)).(filter ((==note).head))) paths
          where paths = (chalan raag direction) ++ consecutivePairs (notes raag direction)
                  where consecutivePairs xs = [take 2 $ drop i xs | i<-[0..(length xs)-2]]

between :: Raag -> Note -> Note -> [Note]
between raag a b = head $ dropWhile ((/=b).last) possibilities
  where possibilities = [walk raag direction a i | i<-[1..]]
        direction = if (a<b) then Accending else Decending

arohi :: Raag -> [Note]
arohi raag = between raag (start raag) (high sa)

avrohi :: Raag -> [Note]
avrohi raag = between raag (high sa) sa

palta :: Raag -> [Int] -> [[[Note]]]
palta raag pattern = [helper Accending, helper Decending]
  where helper direction = case direction of 
          Accending -> paltaUntil (<= high sa) (arohi raag)
          Decending -> paltaUntil (>= sa) (avrohi raag)
          where paltaUntil condition notes = takeWhile (condition.last) (map paltaAt notes)
                paltaAt note = map ((walkNotes !!).(subtract 1)) pattern
                  where walkNotes = walk raag direction note (maximum pattern)


-- TODO doesn't really work for raags with a non empty chalan
loops :: Raag -> Note -> [[Note]]
loops raag note = takeWhile ((<=high note) . maximum) [loop k | k <- [2..]]
  where loop k = accent ++ (tail decent)
          where accent = walk raag Accending note k
                decent = between raag (last accent) note


letter :: Note -> String
letter (Note octave name) = char : diacritic
  where char = case name of 
          Sa -> 'S'
          KomalRe -> 'r'
          Re -> 'R'
          KomalGa -> 'g'
          Ga -> 'G'
          Ma -> 'M'
          TeevraMa -> 'm'
          Pa -> 'P'
          KomalDha -> 'd'
          Dha -> 'D'
          KomalNi -> 'n' 
          Ni -> 'N'
        diacritic = case octave of 
          Low -> [toEnum 803] -- unicode dot below
          Middle -> ""
          High -> [toEnum 775] -- unicode dot above


instance Show Note where
  show = letter

instance {-# OVERLAPPING #-} Show [Note] where
  show = (intercalate " ") . (map letter)

instance {-# OVERLAPPING #-} Show [[Note]] where
  show = (intercalate " ") . ((map concat) . (map (map letter)))

instance {-# OVERLAPPING #-} Show [[[Note]]] where
  show = (intercalate "\n") . (map show)

instance {-# OVERLAPPING #-} Show [[[[Note]]]] where
  show = (intercalate "\n\n") . (map show)
