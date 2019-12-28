let items_struct = undefined;

const load_data = (() => "***");
const readyPromise = new Promise((resolve, reject) => {
    if (document.readyState === 'complete' || (document.readyState !== 'loading' && !document.documentElement.doScroll)) {
        setTimeout(() => resolve(), 1);
    } else {
        const onContentLoaded = () => {
            resolve();
            document.removeEventListener('DOMContentLoaded', onContentLoaded, false);
        }
        document.addEventListener('DOMContentLoaded', onContentLoaded, false);
    }
})
readyPromise.then(() => {
    const dom = {
        createElement: (params) => {
            if (!params) {
                params = {};
            }
            const elementName = params.name || 'div';
            const element = document.createElement(elementName);
            if (params.id) {
                element.id = params.id;
            }
            if (params.className) {
                element.classList.add(params.className);
            }
            if (params.classNames) {
                params.classNames.forEach((className) => element.classList.add(className));
            }
            if (params.parent) {
                params.parent.appendChild(element);
            }
            if (params.text) {
                element.textContent = params.text;
            }
            return element;
        },
        addClass: (element, className) => element.classList.add(className),
        setTextContext: (element, text) => element.textContent = text,
        addMouseOutOverEvent: (element, callback) => {
            element.addEventListener('mouseover', callback);
            element.addEventListener('mouseout', callback);
        },
        addMouseMoveEvent: (element, callback) => element.addEventListener('mousemove', callback),
        addClickEvent: (element, callback) => element.addEventListener('click', callback),
        addResizeEvent: (element, callback) => element.addEventListener('resize', callback),
        hasClass: (element, className) => element.classList.contains(className),
        clear: (element) => element.textContent = '',
        setAttr: (element, name, value) => element.setAttribute(name, value),
        setStyle: (element, name, value) => element.style[name] = value,
        removeIfExists: (element) => {
            if (element) {
                element.remove();
            }
        },
        get_size: (elem) => {
            return [elem.offsetLeft, elem.offsetTop, elem.clientWidth, elem.clientHeight];
        },
        clear_canvas: (canvas) => {
            const context = canvas.getContext('2d');
            context.clearRect(0, 0, canvas.width, canvas.height);
        },
        set_canvas_size: (canvas, size) => {
            dom.setStyle(canvas, 'left', size[0] + 'px');
            dom.setStyle(canvas, 'top', size[1] + 'px');
            dom.setStyle(canvas, 'width', size[2] + 'px');
            dom.setStyle(canvas, 'height', size[3] + 'px');
            dom.setStyle(canvas, 'border', 0);
            canvas.width = size[2];
            canvas.height = size[3];
        },
        context_draw_rect: (context, size, color) => {
            let [a, b, c, d] = size;
            context.beginPath();
            context.strokeStyle = color;
            context.lineWidth = 1;
            context.rect(a, b, c, d);
            context.stroke();
            context.stroke();
            context.closePath();
        },
        context_fill_rect: (context, size, color) => {
            let [a, b, c, d] = size;
            context.beginPath();
            context.fillStyle = color;
            context.rect(a, b, c, d);
            context.fill();
            context.closePath();
        },
    }
    const elements = {
        location: document.getElementById('location'),
        name: document.getElementById('name'),
        size: document.getElementById('size'),
        subdir: document.getElementById('subdir'),
        root: document.getElementById('root'),
        main: document.getElementById('main'),
        up: document.getElementById('up'),
        canvas: document.getElementById('canvas'),
    };
    window.elements = elements;
    window.dom = dom;
    const do_now = (callback) => setTimeout(callback, 0);
    const do_soon = (callback) => setTimeout(callback, 100);
    const positions_by_item = {};
    window.positions_by_item = positions_by_item;
    const apply_size = (size, color, isMain, path, first_level_item) => {
        positions_by_item[path] = { size, first_level_item };
        if (isMain) {
            [elements.canvas, elements.canvasHover].forEach((element) => {
                dom.set_canvas_size(element, size);
            })
            dom.setStyle(elements.canvasHover, 'opacity', 0.5);
            dom.clear_canvas(elements.canvasHover);
        }
        if (undefined !== color) {
            dom.context_fill_rect(elements.context, size, color);
            dom.context_draw_rect(elements.context, size, 'rgba(0,0,0,0.1)');
        }
    };
    const match_position = (point, size) => point[0] >= size[0] && point[0] <= size[0] + size[2] && point[1] >= size[1] && point[1] <= size[1] + size[3];
    const search_item = (root_item, point) => {
        let current_item = root_item;
        let last_item = null;

        while (last_item !== current_item) {
            last_item = current_item;
            if (current_item.children) {
                current_item.children.forEach((child_item) => {
                    if (child_item && child_item.path && positions_by_item[child_item.path]) {
                        if (match_position(point, positions_by_item[child_item.path].size)) {
                            current_item = child_item;
                        }
                        // console.log(`Search [${child_item.path}] => [${match}]`);
                    }
                })
            }
        }

        return current_item;
    }
    const format_number = ((x) => x.toString().replace(/\B(?=(\d{3})+(?!\d))/g, " "));
    const formatNameSize = (name, size) => `${name} : ${size}`;
    const show_item_first_level = (item) => {
        if (item) {
            const name = item.path;
            const size = format_number(item.value);
            const text = formatNameSize(name, size);
            dom.setTextContext(elements.subdir, text);
        } else {
            dom.setTextContext(elements.subdir, '');
        }
    };
    const show_item = (item, do_console) => {
        if (item) {
            const name = item.path;
            const size = format_number(item.value);
            if (do_console) {
                console.log(formatNameSize(name, size));
            };
            dom.setTextContext(elements.name, name);
            dom.setTextContext(elements.size, size);
        } else {
            dom.setTextContext(elements.name, '');
            dom.setTextContext(elements.size, '');
        }
    };
    const get_sub_size = (size) => [size[0] + 1, size[1] + 1, size[2] - 2, size[3] - 2];
    let current_render_id = 0;
    const squarify = (size, items, value_total, is_first_level, first_level_item, render_id) => {
        if (current_render_id !== render_id) {
            return;
        }
        let base_index = 0;
        if (size[2] < 2 || size[3] < 2) {
            return;
        }
        if (undefined === value_total) {
            value_total = 0;
        }
        let base_direction = 0;
        items.sort((a, b) => b.value - a.value);
        if (size[3] > size[2]) {
            base_direction = 1;
        }
        if (0 === value_total) {
            items.forEach((item) => {
                value_total += item.value;
            });
        }
        const lanes = [];
        while (base_index < items.length) {
            let best_index = base_index;
            let best_ratio = -1.0;
            let best_lane = [];
            let value_current_index = 0;
            let current_index = base_index;
            while (current_index < items.length) {
                value_current_index += items[current_index].value;
                const len = value_current_index * size[2 + base_direction] / value_total;
                let worst_ratio = -1.0;
                const lane_sizes = [];
                for (let index = base_index; index <= current_index; index++) {
                    const h = items[index].value * size[2 + (1 - base_direction)] / value_current_index;
                    if (h !== 0 && len !== 0) {
                        let ratio;
                        if (h > len) {
                            ratio = h / len;
                        } else {
                            ratio = len / h;
                        }
                        if (ratio > worst_ratio) {
                            worst_ratio = ratio;
                        }
                        const item = items[index];
                        lane_sizes.push({ item, h, len });
                    }

                }
                const inverted_worst_ratio = 1 / worst_ratio;
                if (inverted_worst_ratio > best_ratio) {
                    best_index = current_index;
                    best_ratio = inverted_worst_ratio;
                    best_lane = {
                        sizes: lane_sizes,
                        len,
                    };
                }
                current_index += 1;
            }
            lanes.push(best_lane);
            base_index = best_index + 1;
        }
        let x = 0;
        lanes.forEach((lane) => {
            let y = 0;
            const len = lane.len;
            const sizes = lane.sizes;
            if (sizes !== undefined) {
                sizes.forEach((size_item) => {
                    const item = size_item.item;
                    const h = size_item.h;

                    if (is_first_level) {
                        first_level_item = item;
                    }
                    let xsize;
                    if (base_direction === 0) {
                        xsize = [size[0] + x, size[1] + y, len, h];
                    } else {
                        xsize = [size[0] + y, size[1] + x, h, len];
                    }
                    apply_size(xsize, item.color, false, item.path, first_level_item);
                    y += h;
                    if (item.is_dir && undefined !== item.children && item.children.length > 0 && item.depth <= items_struct.current.depth + items_struct.depth) {
                        const action = ((size, items, first_level_item) => {
                            do_now(() => {
                                squarify(size, items, undefined, false, first_level_item, render_id);
                            });
                        });
                        action(get_sub_size(xsize), item.children, first_level_item);
                    }
                });
            }
            x += len;
        });
    };
    const type_finder_struct = {
        extension: {
            "~": "tmp",
            bak: "tmp",
            old: "tmp",
            "svn-base": "tmp",
            "svn-work": "tmp",
            c: "dev",
            cpp: "dev",
            cc: "dev",
            h: "dev",
            hpp: "dev",
            el: "dev",
            y: "dev",
            l: "dev",
            py: "dev",
            pl: "dev",
            sh: "dev",
            coffee: "dev",
            js: "dev",
            jade: "dev",
            pug: "dev",
            styl: "dev",
            css: "dev",
            cs: "dev",
            csproj: "dev",
            resx: "dev",
            xaml: "dev",
            java: "dev",
            vcproj: "dev",
            ui: "dev",
            glade: "dev",
            nsi: "dev",
            nsh: "dev",
            bat: "dev",
            cmd: "dev",
            o: "compiled",
            lo: "compiled",
            Po: "compiled",
            al: "compiled",
            "moc.cpp": "compiled",
            "moc.cc": "compiled",
            elc: "compiled",
            la: "compiled",
            a: "compiled",
            rpm: "compiled",
            pyc: "compiled",
            "tar.bz2": "compress",
            "tar.gz": "compress",
            tgz: "compress",
            bz2: "compress",
            bz: "compress",
            gz: "compress",
            "7z": "compress",
            html: "document",
            htm: "document",
            txt: "document",
            doc: "document",
            xml: "document",
            pdf: "document",
            md: "document",
            png: "image",
            jpg: "image",
            jpeg: "image",
            gif: "image",
            tif: "image",
            tiff: "image",
            bmp: "image",
            xpm: "image",
            tga: "image",
            svg: "image",
            xcf: "image",
            psd: "image",
            ico: "image",
            ora: "image",
            psg: "image",
            config: "config",
            conf: "config",
            conf: "config",
            log4net: "config",
            ini: "config",
            wav: "sound",
            mp3: "sound",
            au: "sound",
            avi: "movie",
            mov: "movie",
            mpg: "movie",
            mpeg: "movie",
            wmv: "movie",
            asf: "movie",
            ogm: "movie",
            mkv: "movie",
            mp4: "movie",
            ts: "movie",
            webm: "movie",
            ps: "image",
            exe: "exec",
            com: "exec",
            swf: "exec",
            jar: "exec",
            msi: "exec",
            dll: "lib",
            bpl: "lib",
            zip: "compress",
            rar: "compress",
            arj: "compress",
            iso: "compress",
            ova: "compress",
            bpk: "dev",
            tds: "compiled",
            obj: "compiled"
        },
        contains: {
            ".so.": "lib"
        },
        exact: {
            core: "tmp",
            entries: "tmp",
            Dockerfile: "dev",
            Makefile: "dev",
            README: "document",
            LICENSE: "document"
        },
        endswith: {
            "~": "tmp"
        },
    };
    const color_by_type_struct = {
        tmp: "#FF8A65",
        dev: "#4DD0E1",
        document: "#2196F3",
        compiled: "#00BCD4",
        compress: "#66BB6A",
        image: "#e53935",
        sound: "#FFF176",
        movie: "#D4E157",
        lib: "#c62828",
        exec: "#AD1457",
        file: "#BBDEFB",
        conf: "#AFB42B",
        dir: "#ffffff",
        _: "#6A1B9A",
    };
    const guess_type = (name => {
        var ext, pattern;
        for (ext in type_finder_struct.extension) {
            if (name.endsWith("." + ext)) {
                return type_finder_struct.extension[ext];
            }
        };
        for (pattern in type_finder_struct.contains) {
            if (-1 !== name.indexOf(pattern)) {
                return type_finder_struct.contains[pattern];
            }
        };
        for (pattern in type_finder_struct.exact) {
            if (name === pattern) {
                return type_finder_struct.exact[pattern];
            }
        };
        for (pattern in type_finder_struct.endswith) {
            if (name.endsWith(pattern)) {
                return type_finder_struct.endswith[pattern];
            }
        };
        return "_";
    });
    const get_color_by_type = (name => color_by_type_struct[name]);
    const setStatus = (text) => dom.setTextContext(elements.location, text);
    const apply_items = ((items_struct) => {
        const root_size = dom.get_size(elements.root);
        root_size[0] = 0;
        root_size[1] = 0;
        if (undefined === items_struct) {
            return;
        }
        const root_item = items_struct.current;
        Object.keys(positions_by_item).forEach(key => delete positions_by_item[key]);
        current_render_id += 1;
        apply_size(root_size, null, true, root_item.path, null);
        setStatus(`${root_item.path} : ${format_number(root_item.value)}`);
        squarify(root_size, root_item.children, undefined, true, null, current_render_id);
    });
    const on_data = ((data) => {
        var data_obj, items_by_path;
        data_obj = {};
        items_by_path = {};
        setStatus(`Data loaded. Starting analysing data (${format_number(data.length)})...`);
        do_now(() => {
            data.forEach((item) => {
                if (item.id !== undefined) {
                    item.children = [];
                    item.value = item.occ_size;
                    if (item.is_dir) {
                        item.guessedtype = "dir";
                    } else {
                        item.guessedtype = guess_type(item.name);
                    }
                    item.color = get_color_by_type(item.guessedtype);
                    data_obj[item.id] = item;
                    items_by_path[item.path] = item;
                }
            });
            data.forEach((item) => {
                if (item.parent_id === 0) {
                    item.parent = undefined;
                    items_struct = {
                        items_by_path: items_by_path,
                        data: item,
                        current: item,
                        depth: 5
                    };
                } else {
                    if (undefined !== data_obj[item.parent_id]) {
                        data_obj[item.parent_id].children.push(item);
                        item.parent = data_obj[item.parent_id];
                    }
                }
            });
            setStatus('');
            dom.addClickEvent(elements.up, (e) => {
                if (undefined !== items_struct.current.parent) {
                    items_struct.current = items_struct.current.parent;
                }
                apply_items(items_struct);
            });
            apply_items(items_struct);
        })
    });
    let current_first_level_item = null;
    const on_hover = (e) => {
        const point = [e.offsetX, e.offsetY];
        const item = search_item(items_struct.current, point);
        if (item !== null && e.type !== 'mouseout') {
            const path = item.path;
            let first_level_item = null;
            if (path && positions_by_item[path] && positions_by_item[path].first_level_item) {
                first_level_item = positions_by_item[path].first_level_item;
            }
            if (current_first_level_item !== first_level_item) {
                current_first_level_item = first_level_item;
                show_item_first_level(current_first_level_item);
                dom.clear_canvas(elements.canvasHover);
                if (e.type !== 'mouseout' && items_struct && items_struct.current && items_struct.current.children) {
                    items_struct.current.children.forEach((child_item) => {
                        if (child_item.path !== current_first_level_item.path) {
                            if (positions_by_item[child_item.path]) {
                                dom.context_fill_rect(elements.contextHover, positions_by_item[child_item.path].size, 'black');
                            }
                        }
                    })
                }
            }
            show_item(item);
        } else {
            dom.clear_canvas(elements.canvasHover);
            current_first_level_item = null;
            show_item(null);
            show_item_first_level(null);
        }


    }
    const on_click = (e) => {
        const point = [e.offsetX, e.offsetY];
        const item = search_item(items_struct.current, point);
        if (e.ctrlKey) {
            const path = item.path;
            if (path && positions_by_item[path] && positions_by_item[path].first_level_item) {
                const first_level_item = positions_by_item[path].first_level_item;
                if (first_level_item.is_dir) {
                    items_struct.current = first_level_item;
                    apply_items(items_struct);
                }
            }
        } else {
            show_item(item, true);
        }
        e.stopPropagation();
    }
    const createMainStructure = () => {
        dom.removeIfExists(document.getElementById('workspace'));
        const workspace = dom.createElement({ id: 'workspace', parent: document.body });
        const infos = dom.createElement({ id: 'infos', parent: workspace });
        const labels = dom.createElement({ id: 'labels', parent: infos });
        const location = dom.createElement({ id: 'location', parent: labels, className: 'info' });
        const subdir = dom.createElement({ id: 'subdir', parent: labels, className: 'info' });
        const name = dom.createElement({ id: 'name', parent: labels, className: 'info' });
        const size = dom.createElement({ id: 'size', parent: labels, className: 'info' });
        const buttons = dom.createElement({ id: 'buttons', parent: infos });
        const up = dom.createElement({ id: 'up', parent: buttons, className: 'button', text: '..' });
        const root = dom.createElement({ id: 'root', parent: workspace });
        const canvas = dom.createElement({ id: 'canvas', parent: root, name: 'canvas', className: 'treemaptile' });
        const canvasHover = dom.createElement({ id: 'canvasHover', parent: root, name: 'canvas', className: 'canvasHover' });
        dom.createElement({ name: 'title', parent: document.head, text: 'Treemap' });

        elements.location = location;
        elements.subdir = subdir;
        elements.name = name;
        elements.size = size;
        elements.up = up;
        elements.root = root;
        elements.canvas = canvas;
        elements.canvasHover = canvasHover;
        elements.context = canvas.getContext('2d');
        elements.contextHover = canvasHover.getContext('2d');
        dom.addClickEvent(elements.canvas, on_click);
        dom.addClickEvent(elements.canvasHover, on_click);
        dom.addMouseMoveEvent(elements.canvas, on_hover);
        dom.addMouseMoveEvent(elements.canvasHover, on_hover);
        dom.addMouseOutOverEvent(elements.canvas, on_hover);
        dom.addMouseOutOverEvent(elements.canvasHover, on_hover);
    };
    items_struct = undefined;
    window.apply_items = apply_items;
    createMainStructure();
    setStatus("Loading data...");
    do_now(async () => {
        const data = load_data();
        if (typeof (data) === typeof ('')) {
            try {
                const fetchResponse = await fetch('./map.json');
                const blob = await fetchResponse.blob();
                const text = await blob.text();
                const items = JSON.parse(text);
                window.items = items;
                // console.log("map received", items);
                on_data(items)
            } catch (e) {
                console.log("Loading map failed...");
            }
        } else {
            on_data(data);
        };
        dom.addResizeEvent(window, (e) => apply_items(items_struct));
        apply_items(items_struct)
    })
});
