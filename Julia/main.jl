#kaden Beaver
#main.jl
#Windows 10
#
#enter file name as <name>.txt

using Printf

mutable struct Player
    first::String
    last::String
    plateAppearance::Int64
    atBats::Int64
    singles::Int64
    doubles::Int64
    triples::Int64
    homeruns::Int64
    walks::Int64
    hitByPitch::Int64
    average::Float64
    slugging::Float64
    onBase::Float64
    ops::Float64
    Player() = new("", "", 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0) #default constructor
end

function printPlayerTable(pl::Vector) #prints the table of pl
    @printf("\n\nBASEBALL TEAM REPORT --- %d PLAYERS FOUND IN FILE\n\n", length(playersList))
    @printf("%20s : %12s %12s %12s %8s\n","PLAYER NAME", "AVERAGE", "SLUGGING", "ONBASE%", "OPS")
    @printf("----------------------------------------------------------------------------------\n")
    for players in playersList
        fullname = @sprintf("%s, %s", players.last, players.first)
        @printf("%20s : %12.3f %12.3f %12.3f %8.3f\n", fullname, players.average, players.slugging, players.onBase, players.ops)
    end
end


playersList = Vector{Player}() #creates an empty vector

print("Enter the file name:> \n\n") #propt user for file name

fileName = readline()   #get file name

f= open(fileName, "r")  #open file


for lines in readlines(f)   #get each line
    lineSplit = split(lines, ' ')   #break each line into words
    newPlayer = Player()    #create new player
    newPlayer.first = lineSplit[1]  #and fill in data
    newPlayer.last = lineSplit[2]
    newPlayer.plateAppearance = parse(Int64, lineSplit[3])
    newPlayer.atBats = parse(Int64, lineSplit[4])
    newPlayer.singles = parse(Int64, lineSplit[5])
    newPlayer.doubles = parse(Int64, lineSplit[6])
    newPlayer.triples = parse(Int64, lineSplit[7])
    newPlayer.homeruns = parse(Int64, lineSplit[8])
    newPlayer.walks = parse(Int64, lineSplit[9])
    newPlayer.hitByPitch = parse(Int64, lineSplit[10])
    newPlayer.average = (newPlayer.singles + newPlayer.doubles + newPlayer.triples + newPlayer.homeruns) / newPlayer.atBats #calc statistics
    newPlayer.slugging = (newPlayer.singles + 2*newPlayer.doubles + 3*newPlayer.triples + 4*newPlayer.homeruns) / newPlayer.atBats
    newPlayer.onBase = (newPlayer.singles + newPlayer.doubles + newPlayer.triples + newPlayer.homeruns + newPlayer.walks + newPlayer.hitByPitch) / newPlayer.plateAppearance
    newPlayer.ops = newPlayer.onBase + newPlayer.slugging
    push!(playersList, newPlayer)
end

close(f)

sort!(playersList, by = p -> p.ops, rev = true) #sort by ops in reverse order
printPlayerTable(playersList)
sort!(playersList, by = p -> p.average, rev = true) #sort by average in reverse order
printPlayerTable(playersList)
