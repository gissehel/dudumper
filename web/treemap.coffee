items_struct = undefined

do_now = (callback) => setTimeout callback, 0

apply_size = ($elem, size, color) =>
    $elem.css('left', size[0])
    $elem.css('top', size[1])
    $elem.css('width', size[2])
    $elem.css('height', size[3])
    if color != undefined
        $elem.css('background-color', color)

numberWithCommas = (x) => x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, " ");

show_item = (item, do_console) =>
    name = item.path
    size = numberWithCommas(item.value)
    if do_console
        console.log name + ' : ' + size
    $('#name').text name
    $('#size').text size
    
add_hover = ($elem, item) =>
    $elem.hover (e) =>
        if e.type == 'mouseenter' or e.type == 'mouseover'
            show_item(item)
            e.stopPropagation()
    $elem.click (e) =>
        if e.ctrlKey
            current_item = item
            while (current_item != undefined) and (current_item.parent != undefined) and (current_item.parent != items_struct.current)
                current_item = current_item.parent
            if (current_item.parent == items_struct.current) and (current_item.is_dir)
                items_struct.current = current_item
            apply_items items_struct
            # console.log 'Down'
            e.stopPropagation()
        else
            show_item(item, true)
            e.stopPropagation()

get_size = (elem) =>
    $elem = $(elem)
    position = $elem.position()
    return [position.left, position.top, $elem.innerWidth(), $elem.innerHeight()]

squarify = ($elem, size, items, value_total, is_first_level) =>
    $elem.html('')
    base_index = 0
    base_len = 0
    if value_total == undefined
        value_total = 0
    base_direction = 0
    items.sort (a,b) => b.value-a.value
    if size[3] > size[2]
        base_direction = 1
    if value_total == 0
        for item in items
            value_total += item.value
    lanes = []
    while base_index < items.length
        best_index = base_index
        best_ratio = -1.0
        best_lane = []
        value_current_index = 0
        current_index = base_index
        
        while current_index < items.length
            value_current_index += items[current_index].value
            len = value_current_index * size[2+base_direction] / value_total
            worst_ratio = -1.0
            lane_sizes = []
            for index in [base_index..current_index]
                h = items[index].value * size[2+(1-base_direction)] / value_current_index
                if h != 0 and len != 0
                    if h>len
                        ratio = h/len
                    else
                        ratio = len/h
                    if ratio > worst_ratio
                        worst_ratio = ratio
                    lane_sizes.push {
                            item: items[index]
                            h:h
                            len:len
                        }
            inverted_worst_ratio = 1/worst_ratio
            if inverted_worst_ratio > best_ratio
                best_index = current_index
                best_ratio = inverted_worst_ratio
                best_lane = {
                        sizes:lane_sizes
                        len:len
                    }
                
            current_index += 1
        lanes.push best_lane
        base_index = best_index+1
    
    x = 0
    for lane in lanes
        y = 0
        len = lane.len
        sizes = lane.sizes
        if sizes != undefined
            for size in sizes
                item = size.item
                h = size.h
                $newelem = $('<div/>')
                $newelem.addClass 'treemaptile'
                if is_first_level
                    $newelem.addClass 'first_level'
                if base_direction == 0
                    xsize = [x,y,len,h]
                else
                    xsize = [y,x,h,len]
                apply_size $newelem, xsize, item.color
                y += h
                $newelem.attr('x-data-path',item.path)
                $elem.append $newelem
            
                # Gotcha: here, a function should be called, not directly $newelem.hover as item.path will always get the last version of item, not the correct one
                add_hover $newelem, item
                if (item.is_dir) and (item.children != undefined) and (item.children.length > 0) and (item.depth <= items_struct.current.depth + items_struct.depth)
                    action = ($elem, size, items) =>
                        do_now () =>
                            squarify $elem, size, items, undefined, false
                    action $newelem, (get_size $newelem), item.children
        x += len

type_finder_struct =
    extension: 
        "~"         : 'tmp'
        "bak"       : 'tmp'
        "svn-base"  : 'tmp'
        "svn-work"  : 'tmp'
        "c"         : 'dev'
        "cpp"       : 'dev'
        "cc"        : 'dev'
        "h"         : 'dev'
        "hpp"       : 'dev'
        "el"        : 'dev'
        "y"         : 'dev'
        "l"         : 'dev'
        "py"        : 'dev'
        "pl"        : 'dev'
        "sh"        : 'dev'
        "coffee"    : 'dev'
        "js"        : 'dev'
        "jade"      : 'dev'
        "pug"       : 'dev'
        "styl"      : 'dev'
        "css"       : 'dev'
        
        "o"         : 'compiled'
        "lo"        : 'compiled'
        "Po"        : 'compiled'
        "al"        : 'compiled'
        "moc.cpp"   : 'compiled'
        "moc.cc"    : 'compiled'
        "elc"       : 'compiled'
        "la"        : 'compiled'
        "a"         : 'compiled'
        "rpm"       : 'compiled'
        "pyc"       : 'compiled'
        
        "tar.bz2"   : 'compress' 
        "tar.gz"    : 'compress' 
        "tgz"       : 'compress' 
        "bz2"       : 'compress' 
        "bz"        : 'compress' 
        "gz"        : 'compress' 
        "html"      : 'document' 
        "htm"       : 'document' 
        "txt"       : 'document' 
        "doc"       : 'document' 
        "png"       : 'image'    
        "jpg"       : 'image'    
        "jpeg"      : 'image'    
        "gif"       : 'image'    
        "tif"       : 'image'    
        "tiff"      : 'image'    
        "bmp"       : 'image'    
        "xpm"       : 'image'    
        "tga"       : 'image'    
        "svg"       : 'image'    
        "wav"       : 'sound'    
        "mp3"       : 'sound'    
        "avi"       : 'movie'    
        "mov"       : 'movie'    
        "mpg"       : 'movie'    
        "mpeg"      : 'movie'    
        "wmv"       : 'movie'
        "asf"       : 'movie'
        "ogm"       : 'movie'
        "mkv"       : 'movie'
        "mp4"       : 'movie'
        "pdf"       : 'document'
        "ps"        : 'image'

        "exe"       : 'exec'
        "com"       : 'exec'
        "dll"       : 'lib'
        "zip"       : 'compress'
        "rar"       : 'compress'
        "arj"       : 'compress'
        "iso"       : 'compress'

        "bpk"       : 'dev'
        "tds"       : 'compiled'
        "obj"       : 'compiled'
        "bpl"       : 'lib'
    contains:
        ".so." : 'lib'
    exact:
        "core" : 'tmp'
        "entrier" : 'tmp'

color_by_type_struct =
         'tmp'       : 'orange'
         'dev'       : 'slateblue'
         'document'  : 'blue'
         'compiled'  : 'darkblue'
         'compress'  : 'green'
         'image'     : 'darkred'
         'sound'     : 'yellow'
         'movie'     : '#a0ff00'
         'lib'       : '#ffa0a0'
         'exec'      : 'magenta'
         'file'      : 'lightblue'
         'dir'       : 'white'
         '_'         : 'purple'

guess_type = (name) =>
    for ext of type_finder_struct.extension
        if name.endsWith("."+ext)
            return type_finder_struct.extension[ext]
    for pattern of type_finder_struct.contains
        if name.indexOf(pattern) != -1
            return type_finder_struct.contains[pattern]
    for pattern of type_finder_struct.exact
        if name == pattern
            return type_finder_struct.exact[pattern]
    return '_'

get_color_by_type = (name) =>
    return color_by_type_struct[name]

apply_items = (items_struct) =>
    $root = $('#root')
    $main = $('#main')
    $location = $('#location')
    
    root_size = get_size $root
    root_size[0] = 0
    root_size[1] = 0
    apply_size $main, root_size
    
    if items_struct == undefined
        return
    
    root_item = items_struct.current
    $location.text(root_item.path + ' : ' + numberWithCommas(root_item.value))
    squarify $main, root_size, root_item.children, undefined, true
    # $sub = $($main.children()[2])
    # squarify $sub, (get_size $sub), items.children
    
on_data = (data) =>
        data_obj = {}
        items_by_path = {}
        # console.log "Starting analysing data..."
        $('#location').text("Data loaded. Starting analysing data ("+numberWithCommas(data.length)+")...")
        do_now () =>
            # console.log "Starting analysing data..."
            for item in data
                if item.id != undefined
                    item.children = []
                    item.value = item.occ_size
                    if item.is_dir
                        item.guessedtype = 'dir'
                    else
                        item.guessedtype = guess_type item.name
                    item.color = get_color_by_type item.guessedtype
                    data_obj[item.id] = item
                    items_by_path[item.path] = item
        
            for item in data
                if item.parent_id == 0
                    item.parent = undefined
                    items_struct =
                        items_by_path: items_by_path
                        data: item
                        current: item
                        depth: 3
                else
                    if data_obj[item.parent_id] != undefined
                        data_obj[item.parent_id].children.push item
                        item.parent = data_obj[item.parent_id]
            # console.log "End analysing data..."
            $('#location').text("")
            $('#up').addClass 'button'
            $('#up').click (e) =>
                if items_struct.current.parent != undefined
                    items_struct.current = items_struct.current.parent
                apply_items items_struct
                # console.log 'Up'
            apply_items items_struct
load_data = () =>
    return "***"

get_default_data = () =>
    return "**"

jQuery ($) =>
    items_struct = undefined

    data = load_data()
    
    if data == "*"+get_default_data()
        
        $('#location').text("Loading data...")
    
        items_load_query = $.ajax
            url: './map.json'
            dataType: 'json'

        window.items_load_query = items_load_query

        items_load_query.done (data) =>
            console.log ["map received", data]
            on_data data

        items_load_query.fail () =>
            console.log 'Loading map failed...'
    else
        on_data data

    $(window).on 'resize', (e) =>
       apply_items items_struct
    apply_items items_struct
    
