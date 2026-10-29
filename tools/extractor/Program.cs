// extractor: dump one MapleStory map's geometry from Map.wz to JSON.
// usage: dotnet run -- <path-to-Map.wz> <mapId> <outDir>
//   e.g. dotnet run -- C:/MS/Map.wz 100020000 out

using System.Text.Json;
using MapleLib.WzLib;
using MapleLib.WzLib.WzProperties;

if (args.Length < 3) {
    Console.Error.WriteLine("usage: extractor <Map.wz path> <mapId> <outDir>");
    return 1;
}
string wzPath = args[0], mapId = args[1], outDir = args[2];

var wz = new WzFile(wzPath, WzMapleVersion.GMS);
var status = wz.ParseWzFile();
Console.WriteLine($"parse status: {status}");

// find the map image: Map.wz -> Map -> MapN -> {id}.img
WzImage? FindImage(WzDirectory dir, string name) {
    foreach (var img in dir.WzImages)
        if (img.Name == name) return img;
    foreach (var sub in dir.WzDirectories) {
        var hit = FindImage(sub, name);
        if (hit != null) return hit;
    }
    return null;
}

var img = FindImage(wz.WzDirectory, mapId + ".img");
if (img == null) {
    Console.Error.WriteLine($"image {mapId}.img not found in {wzPath}");
    return 1;
}
img.ParseImage();

WzImageProperty? Get(WzObject? p, string name) =>
    p?[name] as WzImageProperty;
int Int(WzObject? p, string name) =>
    Get(p, name)?.GetInt() ?? 0;
string Str(WzObject? p, string name) =>
    Get(p, name)?.WzValue?.ToString() ?? "";

var result = new Dictionary<string, object>();
result["map_id"] = int.Parse(mapId);
result["name"] = img.Name;

// ---- footholds: foothold -> layer -> group -> fhId -> {x1,y1,x2,y2,prev,next}
var footholds = new List<object>();
var fhRoot = Get(img, "foothold");
if (fhRoot != null) {
    foreach (var layer in fhRoot.WzProperties)
    foreach (var group in layer.WzProperties)
    foreach (var fh in group.WzProperties) {
        footholds.Add(new Dictionary<string, object> {
            ["id"] = int.Parse(fh.Name),
            ["x1"] = Int(fh, "x1"), ["y1"] = Int(fh, "y1"),
            ["x2"] = Int(fh, "x2"), ["y2"] = Int(fh, "y2"),
            ["prev"] = Int(fh, "prev"), ["next"] = Int(fh, "next"),
            ["layer"] = int.Parse(layer.Name),
            ["group"] = int.Parse(group.Name),
        });
    }
}
result["footholds"] = footholds;

// ---- ladders/ropes: ladderRope -> id -> {x,y1,y2,uf,l}
var ladders = new List<object>();
var lrRoot = Get(img, "ladderRope");
if (lrRoot != null) {
    foreach (var l in lrRoot.WzProperties) {
        ladders.Add(new Dictionary<string, object> {
            ["id"] = int.Parse(l.Name),
            ["x"] = Int(l, "x"), ["y1"] = Int(l, "y1"), ["y2"] = Int(l, "y2"),
            ["uf"] = Int(l, "uf"), ["l"] = Int(l, "l"), ["page"] = Int(l, "page"),
        });
    }
}
result["ladders"] = ladders;

// ---- portals: portal -> name -> {x,y,tm,pn,pt}
var portals = new List<object>();
var pRoot = Get(img, "portal");
if (pRoot != null) {
    foreach (var p in pRoot.WzProperties) {
        portals.Add(new Dictionary<string, object> {
            ["id"] = p.Name, ["x"] = Int(p, "x"), ["y"] = Int(p, "y"),
            ["tm"] = Int(p, "tm"), ["pn"] = Str(p, "pn"),
            ["pt"] = Int(p, "pt"),
        });
    }
}
result["portals"] = portals;

// ---- life (spawns): life -> id -> {type, id, x, y, fh, cy}
var spawns = new List<object>();
var lifeRoot = Get(img, "life");
if (lifeRoot != null) {
    foreach (var e in lifeRoot.WzProperties) {
        spawns.Add(new Dictionary<string, object> {
            ["name"] = e.Name,
            ["type"] = Str(e, "type"),
            ["id"] = Str(e, "id"),
            ["x"] = Int(e, "x"), ["y"] = Int(e, "y"),
            ["fh"] = Int(e, "fh"), ["cy"] = Int(e, "cy"),
        });
    }
}
result["spawns"] = spawns;

// ---- map bounds: info -> {VRTop,VRLf,...}
var info = Get(img, "info");
if (info != null) {
    result["bounds"] = new Dictionary<string, object> {
        ["top"] = Int(info, "VRTop"), ["left"] = Int(info, "VRLeft"),
        ["bottom"] = Int(info, "VRBottom"), ["right"] = Int(info, "VRRight"),
    };
}

Directory.CreateDirectory(outDir);
var outPath = Path.Combine(outDir, mapId + ".json");
File.WriteAllText(outPath, JsonSerializer.Serialize(result,
    new JsonSerializerOptions { WriteIndented = true }));

Console.WriteLine($"wrote {outPath}: {footholds.Count} footholds, " +
    $"{ladders.Count} ladders, {portals.Count} portals, {spawns.Count} spawns");
return 0;
