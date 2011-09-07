import Data.Heap as H
import Data.List
import Data.List.Split
import Data.List.Utils as U
import Monad
import System.Environment
import Prelude as P

type IndexItem = (String, [String])

type ParsedFile = [IndexItem]

main = do
    filenames <- getArgs
    fileContents <- mapM readFile filenames
    let merged = mergeFiles fileContents
    putStrLn merged

mergeFiles :: [String] -> String
mergeFiles fileContents =
    let parsedFiles = P.filter (not . P.null) $ map parseFile fileContents :: [ParsedFile]
        heap = H.fromList (map wrap parsedFiles) :: H.MinPrioHeap String ParsedFile
        merged = collapse $ mergeHeap heap
    in unparseFile merged

mergeHeap :: H.MinPrioHeap String ParsedFile -> ParsedFile
mergeHeap heap =
    if H.null heap
       then []
       else let Just ((usr, item:rest), tail) = H.view heap
            in if P.null rest
                  then item : mergeHeap tail
                  else item : (mergeHeap (H.insert (wrap rest) tail))

collapse :: ParsedFile -> ParsedFile
collapse ((x@(usr1, locs1)):(y@(usr2, locs2)):rest)
    | usr1 == usr2 = (usr1, nub (U.merge locs1 locs2)) : collapse rest
    | otherwise    = x : collapse (y:rest)
collapse [x] = [x]
collapse [] = []

wrap :: ParsedFile -> (String, ParsedFile)
wrap x@((usr, locations):rest) = (usr, x)

parseFile :: String -> ParsedFile
parseFile = map parseLine . lines

parseLine :: String -> IndexItem
parseLine str =
    let usr:locations = splitOn "\t" str
    in (usr, locations)

unparseLine :: IndexItem -> String
unparseLine (usr, locs) = usr ++ '\t' : (intercalate "\t" locs)

unparseFile :: ParsedFile -> String
unparseFile = unlines . map unparseLine
