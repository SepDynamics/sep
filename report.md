Found no defects in tracing.cpp
Found no defects in prometheus_exporter.cpp
Found no defects in imgui_impl_glfw.cpp
Found no defects in hip_compat.cpp
Found no defects in imgui_impl_opengl3.cpp
Found no defects in error_handler.cpp
Found no defects in simple_embedding_model.cpp
Found no defects in dag_graph.cpp
Found no defects in metrics_collector.cpp
Found no defects in allocation_metrics.cpp
Found no defects in config_manager_stub.cpp
Found no defects in manager.cpp
Found no defects in stream.cpp
Found no defects in qbsa.cpp
Found no defects in raii.cpp
[LOW] /usr/include/glm/gtc/bitfield.inl:343:15: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x' [deadcode.DeadStores]
    uint16 REG2(x >>= 1);
                ^
  Report hash: 1e773bd29984d4372b430ea9f83aa8cf
  Steps:
    1, bitfield.inl:343:15: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x'

[LOW] /usr/include/glm/gtc/bitfield.inl:397:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x' [deadcode.DeadStores]
    glm::uint32 REG2(x >>= 1);
                     ^
  Report hash: 031d05d8df11fb2b7b4b94b5bf3e5bc6
  Steps:
    1, bitfield.inl:397:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x'

[LOW] /usr/include/glm/gtc/bitfield.inl:451:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x' [deadcode.DeadStores]
    glm::uint64 REG2(x >>= 1);
                     ^
  Report hash: 13c9f8fbeed87c6dad3b9d3f53be2237
  Steps:
    1, bitfield.inl:451:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x'

Found 3 defect(s) in bitfield.inl

Found no defects in qbsa_qfh.cpp
Found no defects in qfh.cpp
Found no defects in memory_tier_manager_serialization.cpp
Found no defects in memory_tier_manager.cpp
[LOW] /sep/third_party/imgui/imgui_internal.h:401:91: Although the value stored to 'c' is used in the enclosing expression, the value is never actually read from 'c' [deadcode.DeadStores]
static inline char      ImToUpper(char c)               { return (c >= 'a' && c <= 'z') ? c &= ~32 : c; }
                                                                                          ^
  Report hash: 3687e255075f6001ae92b283985d0c27
  Steps:
    1, imgui_internal.h:401:91: Although the value stored to 'c' is used in the enclosing expression, the value is never actually read from 'c'

Found 1 defect(s) in imgui_internal.h

[LOW] /sep/third_party/imgui/imgui_tables.cpp:1637:5: Value stored to 'flags' is never read [deadcode.DeadStores]
    flags = column->Flags;
    ^
  Report hash: 2e89b1b05fcaf3a79ee4d16bd8ba7235
  Steps:
    1, imgui_tables.cpp:1637:5: Value stored to 'flags' is never read

[LOW] /sep/third_party/imgui/imgui_tables.cpp:3578:9: Value stored to 'want_separator' is never read [deadcode.DeadStores]
        want_separator = true;
        ^
  Report hash: 9b5c370ee9145f7f256a07573439e6da
  Steps:
    1, imgui_tables.cpp:3578:9: Value stored to 'want_separator' is never read

[LOW] /sep/third_party/imgui/imgui_tables.cpp:3877:67: Value stored to 'line' is never read [deadcode.DeadStores]
        if (sscanf(line, "Sort=%d%c%n", &n, &c, &r) == 2)       { line = ImStrSkipBlank(line + r); column->SortOrder = (ImGuiTableColumnIdx)n; column->SortDirection = (c == '^') ? ImGuiSortDirection_Descending : ImGuiSortDirection_Ascending; settings->SaveFlags |= ImGuiTableFlags_Sortable; }
                                                                  ^
  Report hash: be97a1940a27f64e5f5fb27f1459da94
  Steps:
    1, imgui_tables.cpp:3877:67: Value stored to 'line' is never read

Found 3 defect(s) in imgui_tables.cpp

[HIGH] /usr/lib/clang/20/include/cetintrin.h:49:10: 1st function call argument is an uninitialized value [core.CallAndMessage]
  return __builtin_ia32_rdsspd(t);
         ^
  Report hash: a95dbc87e7d4eb8c0bbf73c1ce08c6bb
  Steps:
    1, cetintrin.h:48:3: 't' declared without an initial value
    2, cetintrin.h:49:10: 1st function call argument is an uninitialized value

[HIGH] /usr/lib/clang/20/include/cetintrin.h:62:10: 1st function call argument is an uninitialized value [core.CallAndMessage]
  return __builtin_ia32_rdsspq(t);
         ^
  Report hash: 076fdab99b1790475cd59e47054be43f
  Steps:
    1, cetintrin.h:61:3: 't' declared without an initial value
    2, cetintrin.h:62:10: 1st function call argument is an uninitialized value

Found 2 defect(s) in cetintrin.h

Found no defects in quantum_processor_qfh.cpp
Found no defects in manifold_config.cpp
[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:3578:13: Variable 'scaling' with floating point type 'float' should not be used as a loop counter [security.FloatLoopCounter]
            for (float scaling = 0.5f; scaling <= 4.0f; scaling += 0.5f)
            ^
  Report hash: ddb0c13d5b577cdbdef0eeeb0e867a4a
  Steps:
    1, imgui_demo.cpp:3578:13: Variable 'scaling' with floating point type 'float' should not be used as a loop counter

[LOW] /sep/third_party/imgui/imgui_demo.cpp:9938:13: Value stored to 'x' is never read [deadcode.DeadStores]
            x += sz + spacing;
            ^
  Report hash: 468f3717dfe4bf78244be42353e714b8
  Steps:
    1, imgui_demo.cpp:9938:13: Value stored to 'x' is never read

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10030:17: Variable 'x' with floating point type 'float' should not be used as a loop counter [security.FloatLoopCounter]
                for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                ^
  Report hash: 6e6d6a6e8af07293f2c03fd769390474
  Steps:
    1, imgui_demo.cpp:10030:17: Variable 'x' with floating point type 'float' should not be used as a loop counter

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10032:17: Variable 'y' with floating point type 'float' should not be used as a loop counter [security.FloatLoopCounter]
                for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                ^
  Report hash: a2bd14bc3bae356a7886e5d6c590c701
  Steps:
    1, imgui_demo.cpp:10032:17: Variable 'y' with floating point type 'float' should not be used as a loop counter

[HIGH] /sep/third_party/imgui/imgui_demo.cpp:10558:29: Dereference of null pointer (loaded from variable 'p_open') [core.NullDereference]
                    *p_open = false;
                            ^
  Report hash: 31b167574b883c4d3b4fab6881aea1de
  Macro expansions:
    1, imgui_demo.cpp:10813:5: Macro 'IMGUI_DEMO_MARKER("Examples/Assets Browser")' expanded to 'do {if (GImGuiDemoMarkerCallback !=__null )GImGuiDemoMarkerCallback ("/sep/third_party/imgui/imgui_demo.cpp",10813,"Examples/Assets Browser",GImGuiDemoMarkerCallbackUserData );}while (0)'
  Steps:
     1, imgui_demo.cpp:10813:5: Assuming the condition is false
     2, imgui_demo.cpp:10815:52: Passing value via 2nd parameter 'p_open'
     3, imgui_demo.cpp:10815:5: Calling 'ExampleAssetsBrowser::Draw'
     4, imgui_demo.cpp:10538:5: Entered call from 'ShowExampleAppAssetsBrowser'
     5, imgui_demo.cpp:10541:13: Assuming the condition is false
     6, imgui_demo.cpp:10548:13: Assuming the condition is true
     7, imgui_demo.cpp:10550:17: Assuming the condition is true
     8, imgui_demo.cpp:10552:21: Assuming the condition is false
     9, imgui_demo.cpp:10554:21: Assuming the condition is false
    10, imgui_demo.cpp:10557:59: Assuming 'p_open' is equal to NULL
    11, imgui_demo.cpp:10557:21: Assuming the condition is true
    12, imgui_demo.cpp:10558:29: Dereference of null pointer (loaded from variable 'p_open')

Found 5 defect(s) in imgui_demo.cpp

Found no defects in quantum_processor_qfh_common.cpp
Found no defects in types_serialization.cpp
Found no defects in evolution.cpp
Found no defects in crow_error.cpp
Found no defects in pattern_processor.cpp
Found no defects in quantum_processor.cpp
Found no defects in pattern_evolution_bridge.cpp
Found no defects in pattern_evolution.cpp
Found no defects in js_integration.cpp
Found no defects in rate_limit_middleware.cpp
Found no defects in processor.cpp
Found no defects in quantum_manifold_optimizer.cpp
Found no defects in auth_middleware.cpp
Found no defects in lock_free_rate_limiter.cpp
Found no defects in dataset_loader.cpp
Found no defects in bridge.cpp
Found no defects in curl_http_client.cpp
Found no defects in crow_request_adapter.cpp
Found no defects in sep_engine_factory.cpp
Found no defects in client.cpp
[HIGH] /sep/third_party/imgui/imgui_draw.cpp:1720:5: Called C++ object pointer is null [core.CallAndMessage]
    font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
    ^
  Report hash: d07e7244950fbacca37f9760aa4638a1
  Steps:
     1, imgui_draw.cpp:1725:5: Calling 'ImDrawList::AddText'
     2, imgui_draw.cpp:1696:1: Entered call from 'ImDrawList::AddText'
     3, imgui_draw.cpp:1698:9: Assuming the condition is false
     4, imgui_draw.cpp:1702:9: Assuming 'text_begin' is not equal to 'text_end'
     5, imgui_draw.cpp:1702:35: Assuming the condition is false
     6, imgui_draw.cpp:1707:9: Assuming pointer value is null
     7, imgui_draw.cpp:1707:9: Assuming 'font' is equal to NULL
     8, imgui_draw.cpp:1708:9: Null pointer value stored to 'font'
     9, imgui_draw.cpp:1709:9: Assuming the condition is false
    10, imgui_draw.cpp:1720:5: Called C++ object pointer is null

[LOW] /sep/third_party/imgui/imgui_draw.cpp:1942:25: Value stored to 'temp' during its initialization is never read [deadcode.DeadStores]
    ImTriangulatorNode* temp = _Nodes;
                        ^
  Report hash: 5c3a00dde3746998b6d30d67580d06ef
  Steps:
    1, imgui_draw.cpp:1942:25: Value stored to 'temp' during its initialization is never read

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5972:9: Variable 'y' with floating point type 'float' should not be used as a loop counter [security.FloatLoopCounter]
        for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
        ^
  Report hash: 0c8f6da39e8d79a495f79fe71358edb4
  Steps:
    1, imgui_draw.cpp:5972:9: Variable 'y' with floating point type 'float' should not be used as a loop counter

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5977:13: Variable 'x' with floating point type 'float' should not be used as a loop counter [security.FloatLoopCounter]
            for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step; x < p_max.x; x += grid_step * 2.0f)
            ^
  Report hash: 00240d8ee7538c2174986ae5b7b6aca2
  Steps:
    1, imgui_draw.cpp:5977:13: Variable 'x' with floating point type 'float' should not be used as a loop counter

Found 4 defect(s) in imgui_draw.cpp

[LOW] /sep/third_party/imgui/imstb_truetype.h:3158:36: Although the value stored to 'xb' is used in the enclosing expression, the value is never actually read from 'xb' [deadcode.DeadStores]
                  t = x0, x0 = xb, xb = t;
                                   ^
  Report hash: 0ecd8410f3027f30a72f6b83d0658905
  Steps:
    1, imstb_truetype.h:3158:36: Although the value stored to 'xb' is used in the enclosing expression, the value is never actually read from 'xb'

Found 1 defect(s) in imstb_truetype.h

Found no defects in pipeline.cpp
[HIGH] /sep/third_party/imgui/imgui.cpp:2322:51: The right operand of '^' is a garbage value [core.UndefinedBinaryOperatorResult]
        crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ *data++];
                                                  ^
  Report hash: 9068fb096a31b143c73a71d390bfcfe0
  Steps:
     1, imgui.cpp:16075:9: Assuming field 'ShowDebugLog' is false
     2, imgui.cpp:16077:9: Assuming field 'ShowIDStackTool' is false
     3, imgui.cpp:16080:9: Assuming the condition is false
     4, imgui.cpp:16080:58: Assuming field 'BeginCount' is <= 1
     5, imgui.cpp:16091:9: Assuming the condition is false
     6, imgui.cpp:16108:9: Assuming field 'ShowWindowsRectsType' is >= 0
     7, imgui.cpp:16110:9: Assuming field 'ShowTablesRectsType' is >= 0
     8, imgui.cpp:16155:9: Assuming the condition is false
     9, imgui.cpp:16261:9: Assuming the condition is false
    10, imgui.cpp:16285:35: Assuming '__begin1' is equal to '__end1'
    11, imgui.cpp:16285:35: Loop body skipped when range is empty
    12, imgui.cpp:16287:9: Assuming the condition is false
    13, imgui.cpp:16298:9: Assuming the condition is true
    14, imgui.cpp:16301:13: Assuming the condition is false
    15, imgui.cpp:16308:39: Assuming '__begin2' is not equal to '__end2'
    16, imgui.cpp:16308:39: Entering loop body
    17, imgui.cpp:16309:13: Calling 'DebugNodeViewport'
    18, imgui.cpp:17135:1: Entered call from 'ShowMetricsWindow'
    19, imgui.cpp:17140:9: Calling 'IsItemHovered'
    20, imgui.cpp:4651:1: Entered call from 'DebugNodeViewport'
    21, imgui.cpp:4657:9: Assuming field 'NavHighlightItemUnderNav' is false
    22, imgui.cpp:4671:13: Assuming the condition is false
    23, imgui.cpp:4683:13: Assuming 'window' is equal to field 'HoveredWindow'
    24, imgui.cpp:4690:17: Assuming field 'ActiveId' is equal to 0
    25, imgui.cpp:4700:13: Assuming the condition is false
    26, imgui.cpp:4706:13: Assuming 'id' is not equal to field 'MoveId'
    27, imgui.cpp:4710:13: Assuming the condition is false
    28, imgui.cpp:4719:9: Assuming the condition is true
    29, imgui.cpp:4721:35: Assuming field 'ID' is equal to 0
    30, imgui.cpp:4721:81: Calling 'ImGuiWindow::GetIDFromPos'
    31, imgui.cpp:8932:1: Entered call from 'IsItemHovered'
    32, imgui.cpp:8936:18: Calling 'ImHashData'
    33, imgui.cpp:2314:1: Entered call from 'ImGuiWindow::GetIDFromPos'
    34, imgui.cpp:2321:12: Entering loop body
    35, imgui.cpp:2321:5: Looping back to the head of the loop
    36, imgui.cpp:2321:12: Entering loop body
    37, imgui.cpp:2322:51: The right operand of '^' is a garbage value

[HIGH] /sep/third_party/imgui/imgui.cpp:7308:36: Dereference of null pointer [core.NullDereference]
        popup_ref.ParentNavLayer = parent_window_in_stack->DC.NavLayerCurrent;
                                   ^
  Report hash: 552ac222b47250a6bab8c8fc7471fd7a
  Macro expansions:
    1, imgui.cpp:7213:5: Macro 'IM_ASSERT(name != NULL && name[0] != '\0')' expanded to '(static_cast <bool >(name !=__null &&name [0]!='\0')?void (0):__assert_fail ("name != __null && name[0] != '\\0'",__builtin_FILE (),__builtin_LINE (),__extension__ __PRETTY_FUNCTION__ ))'
    2, imgui.cpp:7214:5: Macro 'IM_ASSERT(g.WithinFrameScope)' expanded to '(static_cast <bool >(g .WithinFrameScope )?void (0):__assert_fail ("g.WithinFrameScope",__builtin_FILE (),__builtin_LINE (),__extension__ __PRETTY_FUNCTION__ ))'
    3, imgui.cpp:7215:5: Macro 'IM_ASSERT(g.FrameCountEnded != g.FrameCount)' expanded to '(static_cast <bool >(g .FrameCountEnded !=g .FrameCount )?void (0):__assert_fail ("g.FrameCountEnded != g.FrameCount",__builtin_FILE (),__builtin_LINE (),__extension__ __PRETTY_FUNCTION__ ))'
    4, imgui.cpp:7266:5: Macro 'IM_ASSERT(parent_window != NULL || !(flags & ImGuiWindowFlags_ChildWindow))' expanded to '(static_cast <bool >(parent_window !=__null ||!(flags &ImGuiWindowFlags_ChildWindow ))?void (0):__assert_fail ("parent_window != __null || !(flags & ImGuiWindowFlags_ChildWindow)",__builtin_FILE (),__builtin_LINE (),__extension__ __PRETTY_FUNCTION__ ))'
  Steps:
     1, imgui.cpp:7213:5: Assuming the condition is true
     2, imgui.cpp:7213:5: Assuming the condition is true
     3, imgui.cpp:7214:5: Assuming field 'WithinFrameScope' is true
     4, imgui.cpp:7215:5: Assuming field 'FrameCountEnded' is not equal to field 'FrameCount'
     5, imgui.cpp:7219:39: Assuming 'window' is not equal to NULL
     6, imgui.cpp:7224:9: Assuming field 'DebugBreakInWindow' is not equal to field 'ID'
     7, imgui.cpp:7228:9: Assuming the condition is false
     8, imgui.cpp:7232:44: Assuming 'current_frame' is equal to field 'LastFrameActive'
     9, imgui.cpp:7233:33: Assuming field 'Size' is not equal to 0
    10, imgui.cpp:7236:43: Assuming the condition is false
    11, imgui.cpp:7237:9: Assuming the condition is false
    12, imgui.cpp:7264:43: Assuming the condition is true
    13, imgui.cpp:7264:5: 'parent_window_in_stack' initialized to a null pointer value
    14, imgui.cpp:7266:5: Assuming the condition is true
    15, imgui.cpp:7269:9: Assuming field 'Size' is not equal to 0
    16, imgui.cpp:7278:50: Assuming the condition is false
    17, imgui.cpp:7282:9: Assuming the condition is false
    18, imgui.cpp:7300:20: Assuming the condition is false
    19, imgui.cpp:7304:9: Assuming the condition is true
    20, imgui.cpp:7308:36: Dereference of null pointer

[HIGH] /sep/third_party/imgui/imgui.cpp:13650:9: Forming reference to null pointer [core.NonNullParamChecker]
        g.NavScoringNoClipRect = window->InnerRect;
        ^
  Report hash: 4941fc669696c7edb8e52db3a231b0ab
  Steps:
    1, imgui.cpp:13613:5: 'window' initialized here
    2, imgui.cpp:13614:37: Assuming the condition is false
    3, imgui.cpp:13615:38: Assuming the condition is false
    4, imgui.cpp:13617:9: Assuming field 'NavMoveForwardToNextFrame' is false
    5, imgui.cpp:13631:13: Assuming 'window' is null
    6, imgui.cpp:13648:9: Assuming the condition is true
    7, imgui.cpp:13650:9: Forming reference to null pointer

[HIGH] /sep/third_party/imgui/imgui.cpp:14120:13: Array access (via field 'NavLastIds') results in a null pointer dereference [core.NullDereference]
        if (apply_focus_window->NavLastIds[0] == 0)
            ^
  Report hash: 414571462893f93b50a2840ffff8af19
  Steps:
    1, imgui.cpp:14113:9: Assuming field 'NavWindow' is equal to NULL
    2, imgui.cpp:14119:9: Null pointer value stored to 'apply_focus_window'
    3, imgui.cpp:14120:13: Array access (via field 'NavLastIds') results in a null pointer dereference

[LOW] /sep/third_party/imgui/imgui.cpp:17108:5: Value stored to 'p' is never read [deadcode.DeadStores]
    p += ImFormatString(p, buf_end - p, (tab_bar->Tabs.Size > 3) ? " ... }" : " } ");
    ^
  Report hash: f85499f464062e82422271df8e6ef884
  Steps:
    1, imgui.cpp:17108:5: Value stored to 'p' is never read

Found 5 defect(s) in imgui.cpp

Found no defects in cmake_pch.hxx.cxx
Found no defects in ollama_client.cpp
Found no defects in factory.cpp
[HIGH] /usr/include/spa-0.2/spa/pod/parser.h:496:8: Access to field 'type' results in a dereference of a null pointer (loaded from variable 'pod') [core.NullDereference]
      if (pod->type == SPA_TYPE_Choice && *format != 'V')
          ^
  Report hash: 6396cb440c40f5db85f1a9ee33c1714c
  Macro expansions:
    1, parser.h:454:1: Macro 'SPA_API_POD_PARSER' expanded to 'static inline '
  Steps:
     1, parser.h:513:8: Calling 'spa_pod_parser_getv'
     2, parser.h:454:1: Entered call from 'spa_pod_parser_get'
     3, parser.h:457:26: Assuming 'f' is non-null
     4, parser.h:466:12: Assuming 'ftype' is equal to SPA_TYPE_Object
     5, parser.h:470:8: Assuming 'key' is not equal to 0
     6, parser.h:475:10: Assuming 'prop' is null
     7, parser.h:475:4: Null pointer value stored to 'pod'
     8, parser.h:478:7: Assuming the condition is false
     9, parser.h:484:20: Assuming the condition is false
    10, parser.h:484:8: Assuming 'optional' is false
    11, parser.h:487:7: Assuming the condition is false
    12, parser.h:496:8: Access to field 'type' results in a dereference of a null pointer (loaded from variable 'pod')

Found 1 defect(s) in parser.h

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:8294:16: Dereference of null pointer [core.NullDereference]
        while (it->val_i == 0 && it < it_end)
               ^
  Report hash: 58eb83e208071bd835922f5735855403
  Macro expansions:
    1, imgui_widgets.cpp:8292:5: Macro 'IM_ASSERT(it >= _Storage.Data.Data && it <= it_end)' expanded to '(static_cast <bool >(it >=_Storage .Data .Data &&it <=it_end )?void (0):__assert_fail ("it >= _Storage.Data.Data && it <= it_end",__builtin_FILE (),__builtin_LINE (),__extension__ __PRETTY_FUNCTION__ ))'
  Steps:
    1, imgui_widgets.cpp:8288:9: Assuming field 'PreserveOrder' is true
    2, imgui_widgets.cpp:8288:26: Assuming 'it' is equal to NULL
    3, imgui_widgets.cpp:8288:40: Assuming 'it_end' is equal to NULL
    4, imgui_widgets.cpp:8291:9: Null pointer value stored to 'it'
    5, imgui_widgets.cpp:8294:16: Dereference of null pointer

Found 1 defect(s) in imgui_widgets.cpp

[HIGH] /sep/third_party/imgui/imstb_textedit.h:988:33: The left operand of '==' is a garbage value [core.UndefinedBinaryOperatorResult]
            if (find.prev_first == find.first_char)
                                ^
  Report hash: ba8fdbff9ad4feede8253e5429c983f3
  Steps:
     1, imstb_textedit.h:982:10: Calling 'stb_textedit_find_charpos'
     2, imstb_textedit.h:550:1: Entered call from 'stb_textedit_key'
     3, imstb_textedit.h:557:8: Assuming 'n' is equal to 'z'
     4, imstb_textedit.h:557:18: Assuming 'single_line' is not equal to 0
     5, imstb_textedit.h:565:7: Returning without writing to 'find->prev_first'
     6, imstb_textedit.h:982:10: Returning from 'stb_textedit_find_charpos'
     7, imstb_textedit.h:984:22: Assuming 'j' is < 'row_count'
     8, imstb_textedit.h:984:22: Entering loop body
     9, imstb_textedit.h:985:32: Assuming field 'has_preferred_x' is 0
    10, imstb_textedit.h:988:33: The left operand of '==' is a garbage value

Found 1 defect(s) in imstb_textedit.h

Found no defects in workbench_main.cpp
Found no defects in logging.cpp
Found no defects in memory_tier.cpp
Found no defects in engine.cpp
Found no defects in quantum_coherence_manager.cpp
Found no defects in pattern_processor_interface.cpp
Found no defects in redis_manager.cpp
Found no defects in logging_middleware.cpp
Found no defects in annealing_demo.cpp
Found no defects in annealing_sim.cpp
Found no defects in audio_visualizer.cpp
Found no defects in audio_visualizer_simple.cpp
Found no defects in cosmo_demo.cpp
Found no defects in demo_manager.cpp
Found no defects in config.cpp
Found no defects in cosmo_sim.cpp
Found no defects in digital_physics_demo.cpp
Found no defects in crow_adapter.cpp
Found no defects in drug_optimizer.cpp
Found no defects in drug_discovery_demo.cpp
Found no defects in sep_engine.cpp
Found no defects in flocking_demo.cpp
Found no defects in flocking_sim_simple.cpp
Found no defects in genesis_pattern.cpp
Found no defects in memory_garden.cpp
Found no defects in neural_demo.cpp
Found no defects in api.cpp
Found no defects in neuro_sim.cpp
Found no defects in renderer.cpp
Found no defects in physics_explorer_simple.cpp
[MEDIUM] /sep/extern/cycles/third_party/cuew/src/cuew.c:950:26: Potential buffer overflow. Replace with 'sizeof(command) - strlen(command) - 1' or use a safer 'strlcat' API [unix.cstring.BadSizeArg]
  strncat(command, path, sizeof(command) - 1);
                         ^
  Report hash: efa2deebb96beaa86992533d067b3a8d
  Steps:
    1, cuew.c:950:26: Potential buffer overflow. Replace with 'sizeof(command) - strlen(command) - 1' or use a safer 'strlcat' API

Found 1 defect(s) in cuew.c

[MEDIUM] /sep/extern/cycles/third_party/hipew/src/hipew.c:597:26: Potential buffer overflow. Replace with 'sizeof(command) - strlen(command) - 1' or use a safer 'strlcat' API [unix.cstring.BadSizeArg]
  strncat(command, path, sizeof(command) - 1);
                         ^
  Report hash: efa2deebb96beaa86992533d067b3a8d
  Steps:
    1, hipew.c:597:26: Potential buffer overflow. Replace with 'sizeof(command) - strlen(command) - 1' or use a safer 'strlcat' API

Found 1 defect(s) in hipew.c

Found no defects in register_demos.cpp
Found no defects in sky_model.cpp
Found no defects in compression.cpp
Found no defects in sky_nishita.cpp
Found no defects in blender_integration.cpp
[MEDIUM] /sep/third_party/imgui/imgui.h:1540:101: 'ImGuiKey_I' is confusable with 'ImGuiKey_1' [misc-confusable-identifiers]
    ImGuiKey_A, ImGuiKey_B, ImGuiKey_C, ImGuiKey_D, ImGuiKey_E, ImGuiKey_F, ImGuiKey_G, ImGuiKey_H, ImGuiKey_I, ImGuiKey_J,
                                                                                                    ^
  Report hash: 70b9c123374c63eb6c7d4c02420b48dd
  Steps:
    1, imgui.h:1539:17: other declaration found here
    2, imgui.h:1540:101: 'ImGuiKey_I' is confusable with 'ImGuiKey_1'

[MEDIUM] /sep/third_party/imgui/imgui.h:1541:53: 'ImGuiKey_O' is confusable with 'ImGuiKey_0' [misc-confusable-identifiers]
    ImGuiKey_K, ImGuiKey_L, ImGuiKey_M, ImGuiKey_N, ImGuiKey_O, ImGuiKey_P, ImGuiKey_Q, ImGuiKey_R, ImGuiKey_S, ImGuiKey_T,
                                                    ^
  Report hash: 7851b021d302e264139edace21e1c4bf
  Steps:
    1, imgui.h:1539:5: other declaration found here
    2, imgui.h:1541:53: 'ImGuiKey_O' is confusable with 'ImGuiKey_0'

[MEDIUM] /sep/third_party/imgui/imgui.h:2196:25: swap functions should be marked noexcept [performance-noexcept-swap]
    inline void         swap(ImVector<T>& rhs)              { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; T* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }
                        ^
  Report hash: 27ca48338ec824e0ccd2209f617d2d35
  Notes:
    1, imgui.h:2196:62: noexcept  (fixit)
  Steps:
    1, imgui.h:2196:25: swap functions should be marked noexcept

[HIGH] /sep/third_party/imgui/imgui.h:2202:151: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'? [bugprone-sizeof-expression]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                      ^
  Report hash: e898e1f28026d15227a9f7903d5a9624
  Steps:
    1, imgui.h:2140:61: expanded from macro 'IM_ALLOC'
    2, imgui.h:2202:151: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'?

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: do not use 'memcpy' to cast between pointers [bugprone-bitwise-pointer-cast]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 44790e4abe78d7bf277d78278d2e7948
  Steps:
    1, imgui.h:2202:175: do not use 'memcpy' to cast between pointers

[HIGH] /sep/third_party/imgui/imgui.h:2202:213: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'? [bugprone-sizeof-expression]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                                                                    ^
  Report hash: c4cbff7e4be72f41e5f67e258a425529
  Steps:
    1, imgui.h:2202:213: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'?

[MEDIUM] /sep/third_party/imgui/imgui.h:2206:120: do not use 'memcpy' to cast between pointers [bugprone-bitwise-pointer-cast]
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
                                                                                                                       ^
  Report hash: bdf20f6e8f4705e3d9b245cf46376f68
  Steps:
    1, imgui.h:2206:120: do not use 'memcpy' to cast between pointers

[HIGH] /sep/third_party/imgui/imgui.h:2206:144: suspicious usage of 'sizeof()' on an expression of pointer type [bugprone-sizeof-expression]
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
                                                                                                                                               ^
  Report hash: 28aa8a63a6f8b40d2320dc1287819531
  Steps:
    1, imgui.h:2206:144: suspicious usage of 'sizeof()' on an expression of pointer type

[MEDIUM] /sep/third_party/imgui/imgui.h:3152:36: undefined behavior, destination object type 'ImDrawListSplitter' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline ImDrawListSplitter()  { memset(this, 0, sizeof(*this)); }
                                   ^
  Report hash: 958c429996f593e719df4255f31c9f9a
  Steps:
    1, imgui.h:3152:36: undefined behavior, destination object type 'ImDrawListSplitter' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:3282:106: comparing object representation of type 'ImVec2' which does not have a unique object representation; consider comparing the members of the object manually [bugprone-suspicious-memory-comparison]
    inline    void  PathLineToMergeDuplicate(const ImVec2& pos)                 { if (_Path.Size == 0 || memcmp(&_Path.Data[_Path.Size - 1], &pos, 8) != 0) _Path.push_back(pos); }
                                                                                                         ^
  Report hash: 8e0b66bdb779b430dff839695c36a923
  Steps:
    1, imgui.h:3282:106: comparing object representation of type 'ImVec2' which does not have a unique object representation; consider comparing the members of the object manually

[MEDIUM] /sep/third_party/imgui/imgui.h:3445:27: undefined behavior, destination object type 'ImTextureData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImTextureData()     { memset(this, 0, sizeof(*this)); TexID = ImTextureID_Invalid; }
                          ^
  Report hash: d139e047736f142f882de8d021ec7f17
  Steps:
    1, imgui.h:3445:27: undefined behavior, destination object type 'ImTextureData' is not TriviallyCopyable

Found 11 defect(s) in imgui.h

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:767:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    snprintf(node->Name, IM_ARRAYSIZE(node->Name), "%s", name);
    ^
  Report hash: 8cf1442196be101febbd6dfb08d4d46d
  Steps:
    1, imgui_demo.cpp:767:5: cast the expression to void to silence this warning
    2, imgui_demo.cpp:767:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:795:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        snprintf(name_buf, IM_ARRAYSIZE(name_buf), "%s %d", root_names[idx_L0 / root_items_multiplier], idx_L0 % root_items_multiplier);
        ^
  Report hash: ddcece7cdd891655b3a30c5c2d611c31
  Steps:
    1, imgui_demo.cpp:795:9: cast the expression to void to silence this warning
    2, imgui_demo.cpp:795:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:800:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            snprintf(name_buf, IM_ARRAYSIZE(name_buf), "Child %d", idx_L1);
            ^
  Report hash: 2b4648a5b25cf6b1767082c4d20335f0
  Steps:
    1, imgui_demo.cpp:800:13: cast the expression to void to silence this warning
    2, imgui_demo.cpp:800:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:805:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                snprintf(name_buf, IM_ARRAYSIZE(name_buf), "Sub-child %d", 0);
                ^
  Report hash: b967325331077b70ab6a159ca46f4882
  Steps:
    1, imgui_demo.cpp:805:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:805:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2011:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            sprintf(overlay, "avg %f", average);
            ^
  Report hash: 0a640894b7a1807bfed1bb5ba7bc55ea
  Steps:
    1, imgui_demo.cpp:2011:13: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2011:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2060:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        sprintf(buf, "%d/%d", (int)(progress_saturated * 1753), 1753);
        ^
  Report hash: 667e36a07ce1ce0fce0027ad9093f710
  Steps:
    1, imgui_demo.cpp:2060:9: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2060:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2325:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "Item %d", i);
                    ^
  Report hash: bf0db10b21e938e2f5be2f1db79bd42d
  Steps:
    1, imgui_demo.cpp:2325:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2325:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2337:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "Item %d", i);
                    ^
  Report hash: bf0db10b21e938e2f5be2f1db79bd42d
  Steps:
    1, imgui_demo.cpp:2337:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2337:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2398:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(name, "(%.1f,%.1f)", alignment.x, alignment.y);
                    ^
  Report hash: 8453f749bf9244bcc2d2b0f9d1714aae
  Steps:
    1, imgui_demo.cpp:2398:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2398:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2658:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                sprintf(buf, "Object %d", n);
                ^
  Report hash: 68a830ff39fb4a82106f1211105eb579
  Steps:
    1, imgui_demo.cpp:2658:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2658:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2675:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                sprintf(buf, "Object %d", n);
                ^
  Report hash: 68a830ff39fb4a82106f1211105eb579
  Steps:
    1, imgui_demo.cpp:2675:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2675:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2716:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                    ^
  Report hash: 39fea1491db7bd149115f0d3438ba060
  Steps:
    1, imgui_demo.cpp:2716:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2716:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2756:25: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                        sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                        ^
  Report hash: 2508104661a01d9efe6fbed23cbae9c6
  Steps:
    1, imgui_demo.cpp:2756:25: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2756:25: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2818:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "Object %05u: %s", item_id, ExampleNames[item_id % IM_ARRAYSIZE(ExampleNames)]);
                    ^
  Report hash: 8bed36ca6316d5a7bca5a568c4ade177
  Steps:
    1, imgui_demo.cpp:2818:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2818:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2883:25: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                        sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                        ^
  Report hash: 2508104661a01d9efe6fbed23cbae9c6
  Steps:
    1, imgui_demo.cpp:2883:25: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2883:25: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2924:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "Item %d", n);
                    ^
  Report hash: 5fd48bcdc27dd6c2c615a2f59e3e015e
  Steps:
    1, imgui_demo.cpp:2924:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2924:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:2967:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "Object %05d: %s", n, ExampleNames[n % IM_ARRAYSIZE(ExampleNames)]);
                    ^
  Report hash: 39fea1491db7bd149115f0d3438ba060
  Steps:
    1, imgui_demo.cpp:2967:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:2967:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:3257:25: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                        sprintf(label, "Object %05d: %s", item_id, item_category);
                        ^
  Report hash: 48653b3a2a4801e1d1c22fe680fc0fb6
  Steps:
    1, imgui_demo.cpp:3257:25: cast the expression to void to silence this warning
    2, imgui_demo.cpp:3257:25: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:3331:29: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                            sprintf(label, "Delete %d item(s)###DeleteSelected", selection.Size);
                            ^
  Report hash: 26d81ed649620b3e6b638d07e3f8bde8
  Steps:
    1, imgui_demo.cpp:3331:29: cast the expression to void to silence this warning
    2, imgui_demo.cpp:3331:29: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:3509:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    snprintf(name, IM_ARRAYSIZE(name), "%04d", active_tabs[n]);
                    ^
  Report hash: 432cf234728226f29d715da1286511f5
  Steps:
    1, imgui_demo.cpp:3509:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:3509:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[HIGH] /sep/third_party/imgui/imgui_demo.cpp:3578:57: loop induction expression should not have floating-point type [cert-flp30-c]
            for (float scaling = 0.5f; scaling <= 4.0f; scaling += 0.5f)
                                                        ^
  Report hash: 8c200f3c051a124afea0fb8c5fd7f759
  Steps:
    1, imgui_demo.cpp:3578:57: loop induction expression should not have floating-point type

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:4338:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(buf, "%03d", i);
                    ^
  Report hash: 438cb4403792995aeeb0a61133122f7f
  Steps:
    1, imgui_demo.cpp:4338:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:4338:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:4920:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                sprintf(num_buf, "%d", n);
                ^
  Report hash: 8048106088a44e3e70f724641c167c25
  Steps:
    1, imgui_demo.cpp:4920:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:4920:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:5102:13: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
            switch (n)
            ^
  Report hash: f9036715a8e69e832160370bb6ca9f0c
  Steps:
    1, imgui_demo.cpp:5102:13: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:5351:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
            ^
  Report hash: 7641cf20964e6effceb486f26a2e0ea6
  Steps:
    1, imgui_demo.cpp:5351:13: cast the expression to void to silence this warning
    2, imgui_demo.cpp:5351:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:5790:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(buf, "Hello %d,%d", column, row);
                    ^
  Report hash: 1b31f0e3ed9ffeab604fdea68e61c08d
  Steps:
    1, imgui_demo.cpp:5790:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:5790:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:6032:25: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                        sprintf(buf, "Hello %d,%d", column, row);
                        ^
  Report hash: b9a293ae1163679e63b19b99fb90bb22
  Steps:
    1, imgui_demo.cpp:6032:25: cast the expression to void to silence this warning
    2, imgui_demo.cpp:6032:25: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:6180:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                sprintf(label, "Hello %d,%d", column, row);
                ^
  Report hash: 0bfedbc5ff118e4e00a9840df9b891fa
  Steps:
    1, imgui_demo.cpp:6180:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:6180:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:6181:17: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
                switch (contents_type)
                ^
  Report hash: d8f6334efc5711d6f0b5329bf7e33a1a
  Steps:
    1, imgui_demo.cpp:6181:17: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:6389:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            float indent_step = (float)((int)TEXT_BASE_WIDTH / 2);
                                        ^
  Report hash: 22fc50d95b028244654fbf803e000490
  Steps:
    1, imgui_demo.cpp:6389:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:6889:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(buf, "Cell %d,%d", column, row);
                    ^
  Report hash: a6f8bbb924da3238f35b3343c580afe4
  Steps:
    1, imgui_demo.cpp:6889:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:6889:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:7095:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            sprintf(buf, "Synced Table %d", n);
            ^
  Report hash: 88743ba10bc11ffba590e1d5f8cf74f2
  Steps:
    1, imgui_demo.cpp:7095:13: cast the expression to void to silence this warning
    2, imgui_demo.cpp:7095:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:7439:21: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                    sprintf(label, "%04d", item->ID);
                    ^
  Report hash: ad1ed963639c1e97e2cff68dac4d658a
  Steps:
    1, imgui_demo.cpp:7439:21: cast the expression to void to silence this warning
    2, imgui_demo.cpp:7439:21: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:7451:29: 2nd argument 'item_is_selected' (passed to 'selected') looks like it might be swapped with the 3rd, 'selectable_flags' (passed to 'flags') [readability-suspicious-call-argument]
                        if (ImGui::Selectable(label, item_is_selected, selectable_flags, ImVec2(0, row_min_height)))
                            ^
  Report hash: af2fba63bb912f00ca4a91eb6b3d2d92
  Steps:
    1, imgui.h:753:29: in the call to 'Selectable', declared here
    2, imgui_demo.cpp:7451:29: 2nd argument 'item_is_selected' (passed to 'selected') looks like it might be swapped with the 3rd, 'selectable_flags' (passed to 'flags')

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:7551:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            sprintf(label, "Item %d", n);
            ^
  Report hash: d624d9a0f08361b41c0fdeb9cb720de0
  Steps:
    1, imgui_demo.cpp:7551:13: cast the expression to void to silence this warning
    2, imgui_demo.cpp:7551:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:7573:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            sprintf(label, "%04d", i);
            ^
  Report hash: 657220173e48d88beb3fcc97375e9b50
  Steps:
    1, imgui_demo.cpp:7573:13: cast the expression to void to silence this warning
    2, imgui_demo.cpp:7573:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:7949:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
                ^
  Report hash: 7df0f7226b56ea505593ace92f7f184f
  Steps:
    1, imgui_demo.cpp:7949:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:7949:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:8228:9: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
        switch (style_idx)
        ^
  Report hash: 2d5e1a92cf1605696b269ef31d0211ff
  Steps:
    1, imgui_demo.cpp:8228:9: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:8422:50: comparing object representation of type 'ImVec4' which does not have a unique object representation; consider comparing the members of the object manually [bugprone-suspicious-memory-comparison]
                    if (!output_only_modified || memcmp(&col, &ref->Colors[i], sizeof(ImVec4)) != 0)
                                                 ^
  Report hash: 15539c6c98abd5071a7737b19e121dd6
  Steps:
    1, imgui_demo.cpp:8422:50: comparing object representation of type 'ImVec4' which does not have a unique object representation; consider comparing the members of the object manually

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:8459:21: comparing object representation of type 'ImVec4' which does not have a unique object representation; consider comparing the members of the object manually [bugprone-suspicious-memory-comparison]
                if (memcmp(&style.Colors[i], &ref->Colors[i], sizeof(ImVec4)) != 0)
                    ^
  Report hash: 9a9beb4519c47670f4c638303b2a0528
  Steps:
    1, imgui_demo.cpp:8459:21: comparing object representation of type 'ImVec4' which does not have a unique object representation; consider comparing the members of the object manually

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:8787:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        ^
  Report hash: 881cbcd2b1ff40c1bbc8a5a2149597ba
  Steps:
    1, imgui_demo.cpp:8787:9: cast the expression to void to silence this warning
    2, imgui_demo.cpp:8787:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:8986:9: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
        switch (data->EventFlag)
        ^
  Report hash: 64633745c5be42f5ad49a0263ffe5d34
  Steps:
    1, imgui_demo.cpp:8986:9: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:9282:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                sprintf(label, "MyObject %d", i);
                ^
  Report hash: a3bf270f97b7941fe218e22fd32715e5
  Steps:
    1, imgui_demo.cpp:9282:17: cast the expression to void to silence this warning
    2, imgui_demo.cpp:9282:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:9386:25: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
                        switch (field_desc.DataType)
                        ^
  Report hash: 83588cae0811bfe4cd4e33c9f7eb7f65
  Steps:
    1, imgui_demo.cpp:9386:25: switching on non-enum value without default case may not cover all cases

[LOW] /sep/third_party/imgui/imgui_demo.cpp:9507:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (test_type)
    ^
  Report hash: b95db4c102f1ef11ce9c6ca381e46487
  Steps:
    1, imgui_demo.cpp:9507:5: switching on non-enum value without default case may not cover all cases

[HIGH] /sep/third_party/imgui/imgui_demo.cpp:9587:45: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step);
                                            ^
  Report hash: ff61ffc5067294e9d7a7e20b68bdffb6
  Steps:
    1, imgui_demo.cpp:9587:45: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_demo.cpp:9587:94: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step);
                                                                                             ^
  Report hash: 36fee1a4868d642794ae2412a4f3be34
  Steps:
    1, imgui_demo.cpp:9587:94: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:9782:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    sprintf(buf, "Animated title %c %d###AnimatedTitle", "|/-\\"[(int)(ImGui::GetTime() / 0.25f) & 3], ImGui::GetFrameCount());
    ^
  Report hash: 6a96f898b85f901a669e6c849991b6fc
  Steps:
    1, imgui_demo.cpp:9782:5: cast the expression to void to silence this warning
    2, imgui_demo.cpp:9782:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[LOW] /sep/third_party/imgui/imgui_demo.cpp:9833:28: 4th argument 'col_b' (passed to 'col_upr_right') looks like it might be swapped with the 6th, 'col_a' (passed to 'col_bot_left') [readability-suspicious-call-argument]
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                           ^
  Report hash: 2a8bc20fa253273bb015c3ea55bc545a
  Steps:
    1, imgui.h:3246:21: in the call to 'AddRectFilledMultiColor', declared here
    2, imgui_demo.cpp:9833:28: 4th argument 'col_b' (passed to 'col_upr_right') looks like it might be swapped with the 6th, 'col_a' (passed to 'col_bot_left')

[LOW] /sep/third_party/imgui/imgui_demo.cpp:9841:28: 4th argument 'col_b' (passed to 'col_upr_right') looks like it might be swapped with the 6th, 'col_a' (passed to 'col_bot_left') [readability-suspicious-call-argument]
                draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
                           ^
  Report hash: 2a8bc20fa253273bb015c3ea55bc545a
  Steps:
    1, imgui.h:3246:21: in the call to 'AddRectFilledMultiColor', declared here
    2, imgui_demo.cpp:9841:28: 4th argument 'col_b' (passed to 'col_upr_right') looks like it might be swapped with the 6th, 'col_a' (passed to 'col_bot_left')

[HIGH] /sep/third_party/imgui/imgui_demo.cpp:10030:80: loop induction expression should not have floating-point type [cert-flp30-c]
                for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                                                                               ^
  Report hash: d1809b44becde0549814c05636329034
  Steps:
    1, imgui_demo.cpp:10030:80: loop induction expression should not have floating-point type

[HIGH] /sep/third_party/imgui/imgui_demo.cpp:10032:80: loop induction expression should not have floating-point type [cert-flp30-c]
                for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                                                                               ^
  Report hash: 2bcbdadc94f23784ad8c443615b514c3
  Steps:
    1, imgui_demo.cpp:10032:80: loop induction expression should not have floating-point type

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10120:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        snprintf(Name, sizeof(Name), "%s", name);
        ^
  Report hash: aab04aa04224162d8fa2215aaf147c95
  Steps:
    1, imgui_demo.cpp:10120:9: cast the expression to void to silence this warning
    2, imgui_demo.cpp:10120:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10150:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        snprintf(out_buf, out_buf_size, "%s###doc%d", doc->Name, doc->UID);
        ^
  Report hash: a42589043ace1eba8f43303889bc8f78
  Steps:
    1, imgui_demo.cpp:10150:9: cast the expression to void to silence this warning
    2, imgui_demo.cpp:10150:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10194:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        sprintf(buf, "Save %s", doc->Name);
        ^
  Report hash: bd18423055a6d50aaf6150a47c5dee8a
  Steps:
    1, imgui_demo.cpp:10194:9: cast the expression to void to silence this warning
    2, imgui_demo.cpp:10194:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10746:33: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                                sprintf(label, "%d", item_data->ID);
                                ^
  Report hash: c94608a5c391694a7246c007fefb1fcf
  Steps:
    1, imgui_demo.cpp:10746:33: cast the expression to void to silence this warning
    2, imgui_demo.cpp:10746:33: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui_demo.cpp:10797:61: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
                    float hovered_item_rel_pos_y = ((float)(hovered_item_idx / LayoutColumnCount) + fmodf(hovered_item_ny, 1.0f)) * LayoutItemStep.y;
                                                            ^
  Report hash: a9ee23d8e95fcdbada27245d33e9f552
  Steps:
    1, imgui_demo.cpp:10797:61: result of integer division used in a floating point context; possible loss of precision

Found 57 defect(s) in imgui_demo.cpp

[MEDIUM] /sep/third_party/imgui/imgui.cpp:1497:5: undefined behavior, destination object type 'ImGuiIO' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: ff9906a2b61cd9aad1a59dd003c77b78
  Steps:
    1, imgui.cpp:1497:5: undefined behavior, destination object type 'ImGuiIO' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.cpp:1911:5: undefined behavior, destination object type 'ImGuiPlatformIO' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 3bd286e581773c73c557025f1a63b38e
  Steps:
    1, imgui.cpp:1911:5: undefined behavior, destination object type 'ImGuiPlatformIO' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.cpp:2476:15: 'signed char' to 'int' conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
    int len = lengths[*(const unsigned char*)in_text >> 3];
              ^
  Report hash: 97c14c6fc0950226d56ac80b2b160c3c
  Steps:
    1, imgui.cpp:2476:15: 'signed char' to 'int' conversion; consider casting to 'unsigned char' first.

[HIGH] /sep/third_party/imgui/imgui.cpp:2692:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    out  = ((ImU32)IM_F32_TO_INT8_SAT(in.x)) << IM_COL32_R_SHIFT;
                   ^
  Report hash: 616e86b4f9af84e1c633632a70573f0c
  Steps:
    1, imgui_internal.h:276:41: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui.cpp:2692:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui.cpp:2693:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << IM_COL32_G_SHIFT;
                   ^
  Report hash: 25624bccc5c02f725cab5bbf5d072d7d
  Steps:
    1, imgui_internal.h:276:41: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui.cpp:2693:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui.cpp:2694:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << IM_COL32_B_SHIFT;
                   ^
  Report hash: ff2392644827a432f4bd3fd40c450d73
  Steps:
    1, imgui_internal.h:276:41: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui.cpp:2694:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui.cpp:2695:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << IM_COL32_A_SHIFT;
                   ^
  Report hash: 64eb9dd370083d2f9810f80207b22d35
  Steps:
    1, imgui_internal.h:276:41: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui.cpp:2695:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui.cpp:3125:39: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        const int row_increase = (int)((off_y / line_height) + 0.5f);
                                      ^
  Report hash: 4dba003a2db13e55f571f91a0a59c61c
  Steps:
    1, imgui.cpp:3125:39: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui.cpp:3133:5: undefined behavior, destination object type 'ImGuiListClipper' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 8f6896f73d91baa9b0c8d23bab82f68a
  Steps:
    1, imgui.cpp:3133:5: undefined behavior, destination object type 'ImGuiListClipper' is not TriviallyCopyable

[LOW] /sep/third_party/imgui/imgui.cpp:3603:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (idx)
    ^
  Report hash: 39b2f7bed8a3f8c49b5d1a8723f4dee6
  Steps:
    1, imgui.cpp:3603:5: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui.cpp:4406:5: undefined behavior, destination object type 'ImGuiWindow' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 3201b7caa90f0d5858b9582ad681a4c4
  Steps:
    1, imgui.cpp:4406:5: undefined behavior, destination object type 'ImGuiWindow' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.cpp:5557:36: decrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings [bugprone-inc-dec-in-conditions]
    if (g.DebugLocateFrames > 0 && --g.DebugLocateFrames == 0)
                                   ^
  Report hash: dd761e9d6cbd7a4ee32beb8f253b2547
  Steps:
    1, imgui.cpp:5557:11: variable is referenced here
    2, imgui.cpp:5557:36: decrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings

[MEDIUM] /sep/third_party/imgui/imgui.cpp:5562:44: decrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings [bugprone-inc-dec-in-conditions]
    if (g.DebugLogAutoDisableFrames > 0 && --g.DebugLogAutoDisableFrames == 0)
                                           ^
  Report hash: 8a6c0002f7071628c5afd9be43769ba6
  Steps:
    1, imgui.cpp:5562:11: variable is referenced here
    2, imgui.cpp:5562:44: decrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings

[MEDIUM] /sep/third_party/imgui/imgui.cpp:5658:9: do not use 'memcpy' to cast between pointers [bugprone-bitwise-pointer-cast]
        memcpy(builder->Layers[0]->Data + n, layer->Data, layer->Size * sizeof(ImDrawList*));
        ^
  Report hash: 6ad28e532f9db2fe95ed1bac9f33c5b0
  Steps:
    1, imgui.cpp:5658:9: do not use 'memcpy' to cast between pointers

[MEDIUM] /sep/third_party/imgui/imgui.cpp:5813:55: comparing object representation of type 'ImGuiPlatformImeData' which does not have a unique object representation; consider comparing the members of the object manually [bugprone-suspicious-memory-comparison]
    if (g.PlatformIO.Platform_SetImeDataFn != NULL && memcmp(ime_data, &g.PlatformImeDataPrev, sizeof(ImGuiPlatformImeData)) != 0)
                                                      ^
  Report hash: e62f510ab46f157fc9a1062c626e3a84
  Steps:
    1, imgui.cpp:5813:55: comparing object representation of type 'ImGuiPlatformImeData' which does not have a unique object representation; consider comparing the members of the object manually

[MEDIUM] /sep/third_party/imgui/imgui.cpp:6818:67: comparing object representation of type 'ImRect' which does not have a unique object representation; consider comparing the members of the object manually [bugprone-suspicious-memory-comparison]
            if ((window->Flags & ImGuiWindowFlags_ChildWindow) && memcmp(&g.WindowResizeBorderExpectedRect, &border_rect, sizeof(ImRect)) != 0)
                                                                  ^
  Report hash: 9b940f6f38f78911d33cf6db6cfebe44
  Steps:
    1, imgui.cpp:6818:67: comparing object representation of type 'ImRect' which does not have a unique object representation; consider comparing the members of the object manually

[MEDIUM] /sep/third_party/imgui/imgui.cpp:6952:30: 'signed char' to 'const int' conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
        const int border_n = (window->ResizeBorderHeld != -1) ? window->ResizeBorderHeld : window->ResizeBorderHovered;
                             ^
  Report hash: 31745aba97c8248994c0f816ff6a8833
  Steps:
    1, imgui.cpp:6952:30: 'signed char' to 'const int' conversion; consider casting to 'unsigned char' first.

[HIGH] /sep/third_party/imgui/imgui.cpp:7003:57: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
                bg_col = (bg_col & ~IM_COL32_A_MASK) | (IM_F32_TO_INT8_SAT(alpha) << IM_COL32_A_SHIFT);
                                                        ^
  Report hash: 1d7be2429e1c305ff23b1265271382c3
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui.cpp:7003:57: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui.cpp:7040:44: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
                const float border_inner = IM_ROUND(window_border_size * 0.5f);
                                           ^
  Report hash: 870584eb6998ee2d6e6c512fb6a19756
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui.cpp:7040:44: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui.cpp:13108:52: decrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings [bugprone-inc-dec-in-conditions]
        if (can_stop && g.NavTabbingCounter > 0 && --g.NavTabbingCounter == 0)
                                                   ^
  Report hash: 4b20c24cfa7a7f1e20e4ff182ad792ef
  Steps:
    1, imgui.cpp:13108:27: variable is referenced here
    2, imgui.cpp:13108:52: decrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings

[HIGH] /sep/third_party/imgui/imgui.cpp:13508:36: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        const float scroll_speed = IM_ROUND(window->FontRefSize * 100 * io.DeltaTime); // We need round the scrolling speed because sub-pixel scroll isn't reliably supported.
                                   ^
  Report hash: 594cefffd98d08e4cbd6d9c5a672804c
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui.cpp:13508:36: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui.cpp:14928:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        fflush(g.LogFile);
        ^
  Report hash: 55a44c55e6dd46dc1664221f6276b510
  Steps:
    1, imgui.cpp:14928:9: cast the expression to void to silence this warning
    2, imgui.cpp:14928:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/imgui.cpp:17047:94: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size + cell_spacing), base_pos.y + (n / 16) * (cell_size + cell_spacing));
                                                                                             ^
  Report hash: 02ad007e7b33486f093b3e641f4d01fd
  Steps:
    1, imgui.cpp:17047:94: result of integer division used in a floating point context; possible loss of precision

Found 23 defect(s) in imgui.cpp

[MEDIUM] /sep/third_party/imgui/imgui.h:2200:182: undefined behavior, destination object type 'ImGuiListClipperData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
                                                                                                                                                                                     ^
  Report hash: f9e8376525e64a257ecb1d77f77739ec
  Steps:
    1, imgui.h:2200:182: undefined behavior, destination object type 'ImGuiListClipperData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2200:182: undefined behavior, source object type 'const ImGuiListClipperData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
                                                                                                                                                                                     ^
  Report hash: 427be8b53ea6afce9f1133ea91340eb7
  Steps:
    1, imgui.h:2200:182: undefined behavior, source object type 'const ImGuiListClipperData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImGuiListClipperData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: b92fe339d82a0b13893e840625fb1205
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImGuiListClipperData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImGuiListClipperData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 4d5f7ed0d7fd762189aecd599d0ade48
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImGuiListClipperData' is not TriviallyCopyable

[HIGH] /sep/third_party/imgui/imgui.h:2209:210: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'? [bugprone-sizeof-expression]
    inline T*           erase(const T* it)                  { IM_ASSERT(it >= Data && it < Data + Size); const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(T)); Size--; return Data + off; }
                                                                                                                                                                                                                 ^
  Report hash: ddec814a12f2a601c995122243fe52d1
  Steps:
    1, imgui.h:2209:210: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'?

[HIGH] /sep/third_party/imgui/imgui.h:2212:285: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'? [bugprone-sizeof-expression]
    inline T*           insert(const T* it, const T& v)     { IM_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
                                                                                                                                                                                                                                                                                            ^
  Report hash: de36bd1d94661ddf987835bf7ad72767
  Steps:
    1, imgui.h:2212:285: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'?

[MEDIUM] /sep/third_party/imgui/imgui.h:2212:297: do not use 'memcpy' to cast between pointers [bugprone-bitwise-pointer-cast]
    inline T*           insert(const T* it, const T& v)     { IM_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
                                                                                                                                                                                                                                                                                                        ^
  Report hash: 8105bc9b388bd61ee30e4f7db6e88df1
  Steps:
    1, imgui.h:2212:297: do not use 'memcpy' to cast between pointers

[HIGH] /sep/third_party/imgui/imgui.h:2212:320: suspicious usage of 'sizeof()' on an expression of pointer type [bugprone-sizeof-expression]
    inline T*           insert(const T* it, const T& v)     { IM_ASSERT(it >= Data && it <= Data + Size); const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(T)); memcpy(&Data[off], &v, sizeof(v)); Size++; return Data + off; }
                                                                                                                                                                                                                                                                                                                               ^
  Report hash: 1a95024e0688f3af9d543515fcce9dda
  Steps:
    1, imgui.h:2212:320: suspicious usage of 'sizeof()' on an expression of pointer type

Found 8 defect(s) in imgui.h

[HIGH] /sep/third_party/imgui/imgui_internal.h:506:104: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
static inline float  ImRound64(float f)                                         { return (float)(ImS64)(f + 0.5f); }
                                                                                                       ^
  Report hash: 3abdb70a341b25866ef65c0b4b4132a4
  Steps:
    1, imgui_internal.h:506:104: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:784:13: swap functions should be marked noexcept [performance-noexcept-swap]
    void    swap(ImChunkStream<T>& rhs) { rhs.Buf.swap(Buf); }
            ^
  Report hash: 68e75b85f01dfd3b8a3c6bc1055cb55a
  Notes:
    1, imgui_internal.h:784:42: noexcept  (fixit)
  Steps:
    1, imgui_internal.h:784:13: swap functions should be marked noexcept

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:869:47: undefined behavior, destination object type 'ImDrawDataBuilder' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImDrawDataBuilder()                     { memset(this, 0, sizeof(*this)); }
                                              ^
  Report hash: 35bbd1422043f05f7c001ae79fa7a9d5
  Steps:
    1, imgui_internal.h:869:47: undefined behavior, destination object type 'ImDrawDataBuilder' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:1191:43: undefined behavior, destination object type 'ImGuiInputTextDeactivatedState' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImGuiInputTextDeactivatedState()    { memset(this, 0, sizeof(*this)); }
                                          ^
  Report hash: f663a197bc3f96f776d4737ba8ad5138
  Steps:
    1, imgui_internal.h:1191:43: undefined behavior, destination object type 'ImGuiInputTextDeactivatedState' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:1639:39: undefined behavior, destination object type 'ImGuiListClipperData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImGuiListClipperData()          { memset(this, 0, sizeof(*this)); }
                                      ^
  Report hash: 9569001114065355c6a411c6e95f62d1
  Steps:
    1, imgui_internal.h:1639:39: undefined behavior, destination object type 'ImGuiListClipperData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:1830:27: undefined behavior, destination object type 'ImGuiOldColumns' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImGuiOldColumns()   { memset(this, 0, sizeof(*this)); }
                          ^
  Report hash: 885e0c6243705cadde4a61e82124971d
  Steps:
    1, imgui_internal.h:1830:27: undefined behavior, destination object type 'ImGuiOldColumns' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:2114:31: undefined behavior, destination object type 'ImGuiIDStackTool' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImGuiIDStackTool()      { memset(this, 0, sizeof(*this)); CopyToClipboardLastTime = -FLT_MAX; }
                              ^
  Report hash: 3a94fe78b3afeb2298a8eb3e3c57eb69
  Steps:
    1, imgui_internal.h:2114:31: undefined behavior, destination object type 'ImGuiIDStackTool' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:3008:35: undefined behavior, destination object type 'ImGuiTable' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImGuiTable()                { memset(this, 0, sizeof(*this)); LastFrameActive = -1; }
                                  ^
  Report hash: bd9506bce86e061d5b0df21e52ab57f5
  Steps:
    1, imgui_internal.h:3008:35: undefined behavior, destination object type 'ImGuiTable' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:3036:35: undefined behavior, destination object type 'ImGuiTableTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImGuiTableTempData()        { memset(this, 0, sizeof(*this)); LastTimeActive = -1.0f; }
                                  ^
  Report hash: 4567dd7fe2858af2733775391952e1a5
  Steps:
    1, imgui_internal.h:3036:35: undefined behavior, destination object type 'ImGuiTableTempData' is not TriviallyCopyable

[HIGH] /sep/third_party/imgui/imgui_internal.h:3135:69: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    inline float            GetRoundedFontSize(float size) { return IM_ROUND(size); }
                                                                    ^
  Report hash: c6a690f7acf5f4edfab9e31a9cbcce82
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_internal.h:3135:69: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:3806:35: undefined behavior, destination object type 'ImFontAtlasBuilder' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    ImFontAtlasBuilder()        { memset(this, 0, sizeof(*this)); FrameCount = -1; RectsIndexFreeListStart = -1; PackIdMouseCursors = PackIdLinesTexData = -1; }
                                  ^
  Report hash: 413f38c4616afa6e02a7bf4bf312dc4c
  Steps:
    1, imgui_internal.h:3806:35: undefined behavior, destination object type 'ImFontAtlasBuilder' is not TriviallyCopyable

Found 11 defect(s) in imgui_internal.h

[LOW] /sep/third_party/imgui/backends/imgui_impl_glfw.cpp:443:8: 1st argument 'imgui_key' (passed to 'key') looks like it might be swapped with the 2nd, 'keycode' (passed to 'native_keycode') [readability-suspicious-call-argument]
    io.SetKeyEventNativeData(imgui_key, keycode, scancode); // To support legacy indexing (<1.87 user code)
       ^
  Report hash: 3d7d16867bb799c9b0371c7da434b385
  Steps:
    1, imgui.h:2472:21: in the call to 'SetKeyEventNativeData', declared here
    2, imgui_impl_glfw.cpp:443:8: 1st argument 'imgui_key' (passed to 'key') looks like it might be swapped with the 2nd, 'keycode' (passed to 'native_keycode')

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_glfw.cpp:624:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    snprintf(bd->BackendPlatformName, sizeof(bd->BackendPlatformName), "imgui_impl_glfw (%d)", GLFW_VERSION_COMBINED);
    ^
  Report hash: ffb6a65c33ac411fecbbf3b41f511891
  Steps:
    1, imgui_impl_glfw.cpp:624:5: cast the expression to void to silence this warning
    2, imgui_impl_glfw.cpp:624:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_glfw.cpp:927:35: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        current_time = bd->Time + 0.00001f;
                                  ^
  Report hash: 096582b1e101453666f9311c1c2f6ab8
  Steps:
    1, imgui_impl_glfw.cpp:927:35: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in imgui_impl_glfw.cpp

Found no defects in compression_utils.cpp
[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:291:22: both sides of operator are equivalent [misc-redundant-expression]
    if (imgl3wInit() != 0)
                     ^
  Report hash: 1fd9e19ec8550f6690de84a271ba9584
  Steps:
    1, imgui_impl_opengl3.cpp:291:22: both sides of operator are equivalent

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:293:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        ^
  Report hash: 83b3da2e853dc256cc67ebcfc76b63a0
  Steps:
    1, imgui_impl_opengl3.cpp:293:9: cast the expression to void to silence this warning
    2, imgui_impl_opengl3.cpp:293:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:791:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        fprintf(stderr, "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to compile %s! With GLSL: %s\n", desc, bd->GlslVersionString);
        ^
  Report hash: 8b7f5770122a7b0e2a1d587185885aff
  Steps:
    1, imgui_impl_opengl3.cpp:791:9: cast the expression to void to silence this warning
    2, imgui_impl_opengl3.cpp:791:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:797:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        fprintf(stderr, "%s\n", buf.begin());
        ^
  Report hash: 608b151e6695cac6c5330fe36deebb51
  Steps:
    1, imgui_impl_opengl3.cpp:797:9: cast the expression to void to silence this warning
    2, imgui_impl_opengl3.cpp:797:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:810:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        fprintf(stderr, "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to link %s! With GLSL %s\n", desc, bd->GlslVersionString);
        ^
  Report hash: 0bba233dca8fba0bd47d6dfe9bcac7a3
  Steps:
    1, imgui_impl_opengl3.cpp:810:9: cast the expression to void to silence this warning
    2, imgui_impl_opengl3.cpp:810:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:816:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        fprintf(stderr, "%s\n", buf.begin());
        ^
  Report hash: 608b151e6695cac6c5330fe36deebb51
  Steps:
    1, imgui_impl_opengl3.cpp:816:9: cast the expression to void to silence this warning
    2, imgui_impl_opengl3.cpp:816:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/imgui/backends/imgui_impl_opengl3.cpp:840:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    sscanf(bd->GlslVersionString, "#version %d", &glsl_version);
    ^
  Report hash: 73a9d793e988515f34c977d6ea4c8148
  Steps:
    1, imgui_impl_opengl3.cpp:840:5: cast the expression to void to silence this warning
    2, imgui_impl_opengl3.cpp:840:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 7 defect(s) in imgui_impl_opengl3.cpp

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 65c2bee04162edbee8865cc5f3565ce2
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 0855f2b0919c2df74e4712b79a462856
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2211:166: do not use 'memcpy' to cast between pointers [bugprone-bitwise-pointer-cast]
    inline T*           erase_unsorted(const T* it)         { IM_ASSERT(it >= Data && it < Data + Size);  const ptrdiff_t off = it - Data; if (it < Data + Size - 1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
                                                                                                                                                                     ^
  Report hash: f68f74183f1e446c761c8e5c604a1732
  Steps:
    1, imgui.h:2211:166: do not use 'memcpy' to cast between pointers

[HIGH] /sep/third_party/imgui/imgui.h:2211:202: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'? [bugprone-sizeof-expression]
    inline T*           erase_unsorted(const T* it)         { IM_ASSERT(it >= Data && it < Data + Size);  const ptrdiff_t off = it - Data; if (it < Data + Size - 1) memcpy(Data + off, Data + Size - 1, sizeof(T)); Size--; return Data + off; }
                                                                                                                                                                                                         ^
  Report hash: 84f3d180a864b2a7873f49a4a0bea331
  Steps:
    1, imgui.h:2211:202: suspicious usage of 'sizeof(A*)' on pointer-to-aggregate type; did you mean 'sizeof(A)'?

Found 4 defect(s) in imgui.h

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:386:5: undefined behavior, destination object type 'ImDrawListSharedData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 80e78e6aea3098d1d4d9fc3fb6dd4f48
  Steps:
    1, imgui_draw.cpp:386:5: undefined behavior, destination object type 'ImDrawListSharedData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:418:5: undefined behavior, destination object type 'ImDrawList' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 79b4f5d629bde730e61718cfa7a30c88
  Steps:
    1, imgui_draw.cpp:418:5: undefined behavior, destination object type 'ImDrawList' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:576:37: comparing object representation of type 'ImVec4' which does not have a unique object representation; consider comparing the members of the object manually [bugprone-suspicious-memory-comparison]
    if (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &_CmdHeader.ClipRect, sizeof(ImVec4)) != 0)
                                    ^
  Report hash: f7be47344ca5996f7c340b6a0648ff84
  Steps:
    1, imgui_draw.cpp:576:37: comparing object representation of type 'ImVec4' which does not have a unique object representation; consider comparing the members of the object manually

[LOW] /sep/third_party/imgui/imgui_draw.cpp:1738:5: 3rd argument 'uv_min' (passed to 'uv_a') looks like it might be swapped with the 4th, 'uv_max' (passed to 'uv_c') [readability-suspicious-call-argument]
    PrimRectUV(p_min, p_max, uv_min, uv_max, col);
    ^
  Report hash: cc20a6b3bb3331019e4a8452a595f50a
  Steps:
    1, imgui_draw.cpp:757:18: in the call to 'PrimRectUV', declared here
    2, imgui_draw.cpp:1738:5: 3rd argument 'uv_min' (passed to 'uv_a') looks like it might be swapped with the 4th, 'uv_max' (passed to 'uv_c')

[LOW] /sep/third_party/imgui/imgui_draw.cpp:1780:5: 6th argument 'uv_min' (passed to 'uv_a') looks like it might be swapped with the 7th, 'uv_max' (passed to 'uv_b') [readability-suspicious-call-argument]
    ImGui::ShadeVertsLinearUV(this, vert_start_idx, vert_end_idx, p_min, p_max, uv_min, uv_max, true);
    ^
  Report hash: fa78858c85d81c659c60a066ee7474bb
  Steps:
    1, imgui_internal.h:3614:29: in the call to 'ShadeVertsLinearUV', declared here
    2, imgui_draw.cpp:1780:5: 6th argument 'uv_min' (passed to 'uv_a') looks like it might be swapped with the 7th, 'uv_max' (passed to 'uv_b')

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2108:13: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
            memset(&_Channels[i], 0, sizeof(_Channels[i]));  // Current channel is a copy of CmdBuffer/IdxBuffer, don't destruct again
            ^
  Report hash: 171f09d91c728c7439034846a40e928e
  Steps:
    1, imgui_draw.cpp:2108:13: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2132:5: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(&_Channels[0], 0, sizeof(ImDrawChannel));
    ^
  Report hash: a9a766f532b993dd87734cbf0195aecf
  Steps:
    1, imgui_draw.cpp:2132:5: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2225:5: undefined behavior, destination object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&_Channels.Data[_Current]._CmdBuffer, &draw_list->CmdBuffer, sizeof(draw_list->CmdBuffer));
    ^
  Report hash: 106f6382e7c499e18b99f74f4dc32926
  Steps:
    1, imgui_draw.cpp:2225:5: undefined behavior, destination object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2225:5: undefined behavior, source object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&_Channels.Data[_Current]._CmdBuffer, &draw_list->CmdBuffer, sizeof(draw_list->CmdBuffer));
    ^
  Report hash: 309631f7cb2aacda0acead5b7bcf4d01
  Steps:
    1, imgui_draw.cpp:2225:5: undefined behavior, source object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2226:5: undefined behavior, destination object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&_Channels.Data[_Current]._IdxBuffer, &draw_list->IdxBuffer, sizeof(draw_list->IdxBuffer));
    ^
  Report hash: dbe1accd12e4cffac55ae15cc804f662
  Steps:
    1, imgui_draw.cpp:2226:5: undefined behavior, destination object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2226:5: undefined behavior, source object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&_Channels.Data[_Current]._IdxBuffer, &draw_list->IdxBuffer, sizeof(draw_list->IdxBuffer));
    ^
  Report hash: 84cb962aa00ce859d8587a17c7c7d279
  Steps:
    1, imgui_draw.cpp:2226:5: undefined behavior, source object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2228:5: undefined behavior, destination object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&draw_list->CmdBuffer, &_Channels.Data[idx]._CmdBuffer, sizeof(draw_list->CmdBuffer));
    ^
  Report hash: 79c91616181767d5bda352dc91115968
  Steps:
    1, imgui_draw.cpp:2228:5: undefined behavior, destination object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2228:5: undefined behavior, source object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&draw_list->CmdBuffer, &_Channels.Data[idx]._CmdBuffer, sizeof(draw_list->CmdBuffer));
    ^
  Report hash: 581445ac51cbb4a864108f9433a2be82
  Steps:
    1, imgui_draw.cpp:2228:5: undefined behavior, source object type 'ImVector<ImDrawCmd>' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2229:5: undefined behavior, destination object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&draw_list->IdxBuffer, &_Channels.Data[idx]._IdxBuffer, sizeof(draw_list->IdxBuffer));
    ^
  Report hash: 6ebefbec6f573aa1bf37dd55b2de644f
  Steps:
    1, imgui_draw.cpp:2229:5: undefined behavior, destination object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2229:5: undefined behavior, source object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memcpy(&draw_list->IdxBuffer, &_Channels.Data[idx]._IdxBuffer, sizeof(draw_list->IdxBuffer));
    ^
  Report hash: 14c5faca8fdea09fdca61a03c0763b14
  Steps:
    1, imgui_draw.cpp:2229:5: undefined behavior, source object type 'ImVector<ImDrawIdx>' (aka 'ImVector<unsigned short>') is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2626:5: undefined behavior, destination object type 'ImFontAtlas' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 0b54c4b1ad31d011a824268d0e64efe0
  Steps:
    1, imgui_draw.cpp:2626:5: undefined behavior, destination object type 'ImFontAtlas' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2768:13: undefined behavior, destination object type 'ImFontBaked' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
            memcpy(p_dst, p_src, sizeof(ImFontBaked));
            ^
  Report hash: 16fab7d55e6358ecdaff4e988f12e439
  Steps:
    1, imgui_draw.cpp:2768:13: undefined behavior, destination object type 'ImFontBaked' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:2768:13: undefined behavior, source object type 'ImFontBaked' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
            memcpy(p_dst, p_src, sizeof(ImFontBaked));
            ^
  Report hash: 0c51e18e9e8dc1317189fbaa0499afa0
  Steps:
    1, imgui_draw.cpp:2768:13: undefined behavior, source object type 'ImFontBaked' is not TriviallyCopyable

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:3694:64: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        glyph.AdvanceX = space_glyph ? space_glyph->AdvanceX : IM_ROUND(baked->Size * 0.40f);
                                                               ^
  Report hash: c88b5b0349dd38b6fb370e97b298c33a
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_draw.cpp:3694:64: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:4697:26: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            font_off_x = IM_ROUND(font_off_x);
                         ^
  Report hash: 2edecd16aefb09bf7b19ec807b7acfd9
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_draw.cpp:4697:26: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:4699:26: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            font_off_y = IM_ROUND(font_off_y);
                         ^
  Report hash: 337601de55e32c4a04d0114c4d985c8f
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_draw.cpp:4699:26: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:4701:63: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        font_off_y += stbtt__oversample_shift(oversample_v) + IM_ROUND(baked->Ascent);
                                                              ^
  Report hash: b2518709e8f830b2e94e23113aec59dc
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_draw.cpp:4701:63: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5057:5: undefined behavior, destination object type 'ImFontBaked' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: b42f9b5902575f409ca4ec05c8ed8d2e
  Steps:
    1, imgui_draw.cpp:5057:5: undefined behavior, destination object type 'ImFontBaked' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5074:5: undefined behavior, destination object type 'ImFont' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 2c16b0171d1e305e3825db0da2d29fec
  Steps:
    1, imgui_draw.cpp:5074:5: undefined behavior, destination object type 'ImFont' is not TriviallyCopyable

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:5144:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            advance_x = IM_ROUND(advance_x);
                        ^
  Report hash: 7533fc62282f213056f08c43b83b4dbd
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_draw.cpp:5144:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:5176:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            advance_x = IM_ROUND(advance_x);
                        ^
  Report hash: 7533fc62282f213056f08c43b83b4dbd
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_draw.cpp:5176:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5387:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
        unsigned int c = (unsigned int)*s;
                                       ^
  Report hash: 5c042b21fe3588a78209e56d671904bf
  Steps:
    1, imgui_draw.cpp:5387:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first.

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5501:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
        unsigned int c = (unsigned int)*s;
                                       ^
  Report hash: 5c042b21fe3588a78209e56d671904bf
  Steps:
    1, imgui_draw.cpp:5501:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first.

[MEDIUM] /sep/third_party/imgui/imgui_draw.cpp:5678:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
        unsigned int c = (unsigned int)*s;
                                       ^
  Report hash: 5c042b21fe3588a78209e56d671904bf
  Steps:
    1, imgui_draw.cpp:5678:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first.

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:5972:59: loop induction expression should not have floating-point type [cert-flp30-c]
        for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
                                                          ^
  Report hash: 4e4a4de626abe9fabc545bb00fa7cfc2
  Steps:
    1, imgui_draw.cpp:5972:14: floating-point type loop induction variable
    2, imgui_draw.cpp:5972:59: loop induction expression should not have floating-point type

[HIGH] /sep/third_party/imgui/imgui_draw.cpp:5977:86: loop induction expression should not have floating-point type [cert-flp30-c]
            for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step; x < p_max.x; x += grid_step * 2.0f)
                                                                                     ^
  Report hash: 5d7b8bed5b6a4f9f54cbc65993c60dcd
  Steps:
    1, imgui_draw.cpp:5977:86: loop induction expression should not have floating-point type

Found 31 defect(s) in imgui_draw.cpp

[MEDIUM] /sep/third_party/imgui/imgui_internal.h:728:195: undefined behavior, source object type 'const ImFontBaked' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline T*           push_back(const T& v)       { int i = Size; IM_ASSERT(i >= 0); if (Size == Capacity) reserve(Capacity + BLOCK_SIZE); void* ptr = &Blocks[i / BLOCK_SIZE][i % BLOCK_SIZE]; memcpy(ptr, &v, sizeof(v)); Size++; return (T*)ptr; }
                                                                                                                                                                                                  ^
  Report hash: 5cd253eb7302dbf71017d28dbd79ad82
  Steps:
    1, imgui_internal.h:728:195: undefined behavior, source object type 'const ImFontBaked' is not TriviallyCopyable

Found 1 defect(s) in imgui_internal.h

[HIGH] /sep/third_party/imgui/imstb_rectpack.h:297:4: suspicious usage of 'sizeof()' on an expression of pointer type [bugprone-sizeof-expression]
   STBRP__NOTUSED(c);
   ^
  Report hash: c6a17477e63a6b112e90414d651e9672
  Steps:
    1, imstb_rectpack.h:224:34: expanded from macro 'STBRP__NOTUSED'
    2, imstb_rectpack.h:297:4: suspicious usage of 'sizeof()' on an expression of pointer type

Found 1 defect(s) in imstb_rectpack.h

[MEDIUM] /sep/third_party/imgui/imstb_truetype.h:1318:11: comparison between 'signed char' and 'unsigned char' [bugprone-signed-char-misuse]
      if (stbtt_tag(data+loc+0, tag))
          ^
  Report hash: 4b276a12dfbe90871eba1455b8034a6d
  Steps:
    1, imstb_truetype.h:1297:36: expanded from macro 'stbtt_tag'
    2, imstb_truetype.h:1296:90: expanded from macro 'stbtt_tag4'
    3, imstb_truetype.h:1318:11: comparison between 'signed char' and 'unsigned char'

[LOW] /sep/third_party/imgui/imstb_truetype.h:1477:7: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      switch(ttUSHORT(data+encoding_record)) {
      ^
  Report hash: d29c34ab999c89cd76201164058c0bc9
  Steps:
    1, imstb_truetype.h:1477:7: switching on non-enum value without default case may not cover all cases

[LOW] /sep/third_party/imgui/imstb_truetype.h:1479:13: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
            switch (ttUSHORT(data+encoding_record+2)) {
            ^
  Report hash: 24b114500016b596f8214b805919f6e7
  Steps:
    1, imstb_truetype.h:1479:13: switching on non-enum value without default case may not cover all cases

[LOW] /sep/third_party/imgui/imstb_truetype.h:3659:10: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
         switch (vertices[i].type) {
         ^
  Report hash: c3e6244c72d67d75c1fb2ff300a2a48e
  Steps:
    1, imstb_truetype.h:3659:10: switching on non-enum value without default case may not cover all cases

Found 4 defect(s) in imstb_truetype.h

[MEDIUM] /sep/third_party/imgui/imgui.h:2200:182: undefined behavior, destination object type 'ImGuiTableTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
                                                                                                                                                                                     ^
  Report hash: cbb629e9182726f23d7b6cf44cbbe8bc
  Steps:
    1, imgui.h:2200:182: undefined behavior, destination object type 'ImGuiTableTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2200:182: undefined behavior, source object type 'const ImGuiTableTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
                                                                                                                                                                                     ^
  Report hash: 6ad7ab7a22e3e0f75cbf262263d58faa
  Steps:
    1, imgui.h:2200:182: undefined behavior, source object type 'const ImGuiTableTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImGuiOldColumns' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 7faea03a3dfaf77746b14b20b83a148a
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImGuiOldColumns' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImGuiTable' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 115eba8c98264706a536ca485f35b6fd
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImGuiTable' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImGuiTableTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 54f710404b91eb36528ee7487a28f949
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImGuiTableTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImGuiOldColumns' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 023e9ddc89e70c001d7c20aa3edc3507
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImGuiOldColumns' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImGuiTable' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: efac64bf752d4e9d14e6045bd0807768
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImGuiTable' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImGuiTableTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 4050838120481949c5f4623a1cb7d156
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImGuiTableTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2206:120: undefined behavior, destination object type 'ImGuiOldColumns' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
                                                                                                                       ^
  Report hash: 0a869379fd4f338b6c3673f6ea4338b4
  Steps:
    1, imgui.h:2206:120: undefined behavior, destination object type 'ImGuiOldColumns' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2206:120: undefined behavior, source object type 'const ImGuiOldColumns' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         push_back(const T& v)               { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); memcpy(&Data[Size], &v, sizeof(v)); Size++; }
                                                                                                                       ^
  Report hash: 265df02b8342b4dc182b0445c44eae7a
  Steps:
    1, imgui.h:2206:120: undefined behavior, source object type 'const ImGuiOldColumns' is not TriviallyCopyable

Found 10 defect(s) in imgui.h

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2739:21: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
                    memcpy(dst_tmp++, channel, sizeof(ImDrawChannel));
                    ^
  Report hash: f8858bd646c048dbd706287839e7f2ae
  Steps:
    1, imgui_tables.cpp:2739:21: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2739:21: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
                    memcpy(dst_tmp++, channel, sizeof(ImDrawChannel));
                    ^
  Report hash: 2c1939040edebbb218632151807a51d0
  Steps:
    1, imgui_tables.cpp:2739:21: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2745:17: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
                memcpy(dst_tmp++, &splitter->_Channels[table->Bg2DrawChannelUnfrozen], sizeof(ImDrawChannel));
                ^
  Report hash: 834d24310bbf4729f9aa1abb3f62c6e2
  Steps:
    1, imgui_tables.cpp:2745:17: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2745:17: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
                memcpy(dst_tmp++, &splitter->_Channels[table->Bg2DrawChannelUnfrozen], sizeof(ImDrawChannel));
                ^
  Report hash: 9a1b48593fef4a26b7ee50749549a28f
  Steps:
    1, imgui_tables.cpp:2745:17: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2754:13: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
            memcpy(dst_tmp++, channel, sizeof(ImDrawChannel));
            ^
  Report hash: aeddc36522ee7114811772936a2f6335
  Steps:
    1, imgui_tables.cpp:2754:13: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2754:13: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
            memcpy(dst_tmp++, channel, sizeof(ImDrawChannel));
            ^
  Report hash: 034ac5858d220af58a32e177e7d16738
  Steps:
    1, imgui_tables.cpp:2754:13: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2758:9: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
        memcpy(splitter->_Channels.Data + LEADING_DRAW_CHANNELS, g.DrawChannelsTempMergeBuffer.Data, (splitter->_Count - LEADING_DRAW_CHANNELS) * sizeof(ImDrawChannel));
        ^
  Report hash: 7edb0fdafb5be89bcbfcf54b87ce6828
  Steps:
    1, imgui_tables.cpp:2758:9: undefined behavior, destination object type 'ImDrawChannel' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui_tables.cpp:2758:9: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
        memcpy(splitter->_Channels.Data + LEADING_DRAW_CHANNELS, g.DrawChannelsTempMergeBuffer.Data, (splitter->_Count - LEADING_DRAW_CHANNELS) * sizeof(ImDrawChannel));
        ^
  Report hash: 2aba33edfe0ef654ff13d3f9a9007047
  Steps:
    1, imgui_tables.cpp:2758:9: undefined behavior, source object type 'ImDrawChannel' is not TriviallyCopyable

[HIGH] /sep/third_party/imgui/imgui_tables.cpp:4383:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        float clip_x1 = IM_ROUND(window->Pos.x + GetColumnOffset(n));
                        ^
  Report hash: a3e2f6e3d0fb0c19fa43faf9626db6b9
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_tables.cpp:4383:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_tables.cpp:4384:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        float clip_x2 = IM_ROUND(window->Pos.x + GetColumnOffset(n + 1) - 1.0f);
                        ^
  Report hash: 98c60114174ce3b4a0906d51080cf5c6
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_tables.cpp:4384:25: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

Found 10 defect(s) in imgui_tables.cpp

[MEDIUM] /sep/third_party/imgui/imgui.h:2200:182: undefined behavior, destination object type 'ImGuiMultiSelectTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
                                                                                                                                                                                     ^
  Report hash: 990ea84dff5227204ded463d5437c109
  Steps:
    1, imgui.h:2200:182: undefined behavior, destination object type 'ImGuiMultiSelectTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2200:182: undefined behavior, source object type 'const ImGuiMultiSelectTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         resize(int new_size, const T& v)    { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) memcpy(&Data[n], &v, sizeof(v)); Size = new_size; }
                                                                                                                                                                                     ^
  Report hash: 7f9963d1043cbb801681ba18efbf49f7
  Steps:
    1, imgui.h:2200:182: undefined behavior, source object type 'const ImGuiMultiSelectTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImGuiMultiSelectTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 689dbe320c6ae8f919247244fda18154
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImGuiMultiSelectTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, destination object type 'ImGuiTabBar' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: b101e1e3925ef0135611b15b858aab4d
  Steps:
    1, imgui.h:2202:175: undefined behavior, destination object type 'ImGuiTabBar' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImGuiMultiSelectTempData' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: b7aca790634a5c222a572cc3de853b3f
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImGuiMultiSelectTempData' is not TriviallyCopyable

[MEDIUM] /sep/third_party/imgui/imgui.h:2202:175: undefined behavior, source object type 'ImGuiTabBar' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    inline void         reserve(int new_capacity)           { if (new_capacity <= Capacity) return; T* new_data = (T*)IM_ALLOC((size_t)new_capacity * sizeof(T)); if (Data) { memcpy(new_data, Data, (size_t)Size * sizeof(T)); IM_FREE(Data); } Data = new_data; Capacity = new_capacity; }
                                                                                                                                                                              ^
  Report hash: 51484adbdf25c1bad95fb690cc32689b
  Steps:
    1, imgui.h:2202:175: undefined behavior, source object type 'ImGuiTabBar' is not TriviallyCopyable

Found 6 defect(s) in imgui.h

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:963:31: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    const float border_size = IM_ROUND(window->WindowBorderSize * 0.5f);
                              ^
  Report hash: a40f12c72e92982324e0a90702b90bd0
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:963:31: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:964:75: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    const float border_top = (window->Flags & ImGuiWindowFlags_MenuBar) ? IM_ROUND(g.Style.FrameBorderSize * 0.5f) : 0.0f;
                                                                          ^
  Report hash: 987f978ab182c84dd877858c0bbfe4ad
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:964:75: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:1353:16: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    center.x = IM_ROUND(center.x);
               ^
  Report hash: 7a24090542cffda61aff66704facc13f
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:1353:16: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:1354:16: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    center.y = IM_ROUND(center.y);
               ^
  Report hash: 37ca535eb352d45c409222c85f21068b
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:1354:16: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:2280:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (data_type)
    ^
  Report hash: bf7a9dfb2e04500521b4f17572d688b4
  Steps:
    1, imgui_widgets.cpp:2280:5: switching on non-enum value without default case may not cover all cases

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:2388:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (data_type)
    ^
  Report hash: bf7a9dfb2e04500521b4f17572d688b4
  Steps:
    1, imgui_widgets.cpp:2388:5: switching on non-enum value without default case may not cover all cases

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:2417:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (data_type)
    ^
  Report hash: bf7a9dfb2e04500521b4f17572d688b4
  Steps:
    1, imgui_widgets.cpp:2417:5: switching on non-enum value without default case may not cover all cases

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:2644:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (data_type)
    ^
  Report hash: bf7a9dfb2e04500521b4f17572d688b4
  Steps:
    1, imgui_widgets.cpp:2644:5: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui_widgets.cpp:2646:53: 'signed char' to 'ImS32' (aka 'int') conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
    case ImGuiDataType_S8:     { ImS32 v32 = (ImS32)*(ImS8*)p_v;  bool r = DragBehaviorT<ImS32, ImS32, float>(ImGuiDataType_S32, &v32, v_speed, p_min ? *(const ImS8*) p_min : IM_S8_MIN,  p_max ? *(const ImS8*)p_max  : IM_S8_MAX,  format, flags); if (r) *(ImS8*)p_v = (ImS8)v32; return r; }
                                                    ^
  Report hash: 91ac714ad1b1f58379c414076e31434b
  Steps:
    1, imgui_widgets.cpp:2646:53: 'signed char' to 'ImS32' (aka 'int') conversion; consider casting to 'unsigned char' first.

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:3236:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (data_type)
    ^
  Report hash: bf7a9dfb2e04500521b4f17572d688b4
  Steps:
    1, imgui_widgets.cpp:3236:5: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/third_party/imgui/imgui_widgets.cpp:3238:50: 'signed char' to 'ImS32' (aka 'int') conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
    case ImGuiDataType_S8:  { ImS32 v32 = (ImS32)*(ImS8*)p_v;  bool r = SliderBehaviorT<ImS32, ImS32, float>(bb, id, ImGuiDataType_S32, &v32, *(const ImS8*)p_min,  *(const ImS8*)p_max,  format, flags, out_grab_bb); if (r) *(ImS8*)p_v  = (ImS8)v32;  return r; }
                                                 ^
  Report hash: 5a0240f77d08c8cdad4e53be80511f4c
  Steps:
    1, imgui_widgets.cpp:3238:50: 'signed char' to 'ImS32' (aka 'int') conversion; consider casting to 'unsigned char' first.

[MEDIUM] /sep/third_party/imgui/imgui_widgets.cpp:3973:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
        unsigned int c = (unsigned int)*s;
                                       ^
  Report hash: a2ca698c951019ff47bec5bf3f86f056
  Steps:
    1, imgui_widgets.cpp:3973:40: 'signed char' to 'unsigned int' conversion; consider casting to 'unsigned char' first.

[MEDIUM] /sep/third_party/imgui/imgui_widgets.cpp:4214:5: undefined behavior, destination object type 'ImGuiInputTextState' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: ed3ee89bf181c7002b16f62ad4704b85
  Steps:
    1, imgui_widgets.cpp:4214:5: undefined behavior, destination object type 'ImGuiInputTextState' is not TriviallyCopyable

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:4475:42: 3rd argument 'delete_len' (passed to 'insert_len') looks like it might be swapped with the 4th, 'insert_len' (passed to 'delete_len') [readability-suspicious-call-argument]
        if (IMSTB_TEXTEDIT_CHARTYPE* p = stb_text_createundo(&state->Stb->undostate, first_diff, delete_len, insert_len))
                                         ^
  Report hash: 1d34c09d649fc68b1f7eecce70493073
  Steps:
    1, imstb_textedit.h:1244:33: in the call to 'stb_text_createundo', declared here
    2, imgui_widgets.cpp:4475:42: 3rd argument 'delete_len' (passed to 'insert_len') looks like it might be swapped with the 4th, 'insert_len' (passed to 'delete_len')

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:5827:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    ImU32 alpha8 = IM_F32_TO_INT8_SAT(alpha);
                   ^
  Report hash: c58d587d51eccb8bcb6c31d2b834f142
  Steps:
    1, imgui_internal.h:276:41: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:5827:20: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6103:30: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    const int style_alpha8 = IM_F32_TO_INT8_SAT(style.Alpha);
                             ^
  Report hash: 2539b912ffb8591b37420812099dc75e
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6103:30: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6163:35: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        sv_cursor_pos.x = ImClamp(IM_ROUND(picker_pos.x + ImSaturate(S)     * sv_picker_size), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
                                  ^
  Report hash: 2e8b96938f1aca7dfac5166427cdd698
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:6163:35: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6164:35: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        sv_cursor_pos.y = ImClamp(IM_ROUND(picker_pos.y + ImSaturate(1 - V) * sv_picker_size), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);
                                  ^
  Report hash: d161aa0eb85e4701cf6925f9c7ff0f4d
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:6164:35: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6169:29: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        float bar0_line_y = IM_ROUND(picker_pos.y + H * sv_picker_size);
                            ^
  Report hash: 99bebd8c01fc91d10c0953620e01f840
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:6169:29: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6188:29: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        float bar1_line_y = IM_ROUND(picker_pos.y + (1.0f - alpha) * sv_picker_size);
                            ^
  Report hash: 8b4f38d3b3f70c70a5fb86a0d3c7a892
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:6188:29: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6248:23: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        float mid_x = IM_ROUND((bb_inner.Min.x + bb_inner.Max.x) * 0.5f);
                      ^
  Report hash: dd70eb030583a69437bf979b57395c5c
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:6248:23: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6329:14: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
             ^
  Report hash: 17be65877fb4710558b7da071257a495
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6329:14: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6329:47: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                                              ^
  Report hash: d5025a0624c98f23c98dfc2a88b1512c
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6329:47: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6329:80: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                                                                               ^
  Report hash: 5b8465e325e4fba5caa3257c2165b6f9
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6329:80: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6329:159: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                                                                                                                                                              ^
  Report hash: c9e7f847be07710c6e70fa25fef7a9e4
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6329:159: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6379:18: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                 ^
  Report hash: 276f5d5d6310141495eaec1c95507cc6
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6379:18: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6379:51: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                                                  ^
  Report hash: a91809eab8691fa6eff87905d94bc3e2
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6379:51: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6379:84: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                                                                                   ^
  Report hash: e44b49892ffe277ea40f4f5b5a022ba3
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6379:84: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:6379:163: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
        int cr = IM_F32_TO_INT8_SAT(col[0]), cg = IM_F32_TO_INT8_SAT(col[1]), cb = IM_F32_TO_INT8_SAT(col[2]), ca = (flags & ImGuiColorEditFlags_NoAlpha) ? 255 : IM_F32_TO_INT8_SAT(col[3]);
                                                                                                                                                                  ^
  Report hash: 94b3b7d51cd043eb315e0f4879333662
  Steps:
    1, imgui_internal.h:276:47: expanded from macro 'IM_F32_TO_INT8_SAT'
    2, imgui_widgets.cpp:6379:163: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:7084:9: dubious check of 'bool *' against 'nullptr', did you mean to dereference it? [bugprone-bool-pointer-implicit-conversion]
    if (p_visible)
        ^
  Report hash: ac166570feb2debe8ac1ad0ffe7de530
  Notes:
    1, imgui_widgets.cpp:7084:9: * (fixit)
  Steps:
    1, imgui_widgets.cpp:7084:9: dubious check of 'bool *' against 'nullptr', did you mean to dereference it?

[MEDIUM] /sep/third_party/imgui/imgui_widgets.cpp:7418:40: incrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings [bugprone-inc-dec-in-conditions]
    while (s1 < s1_end && ImToUpper(*s1++) == ImToUpper(*s2++))
                                       ^
  Report hash: 124bb41d64dc2a953262dd7eccdb7128
  Steps:
    1, imgui_widgets.cpp:7418:12: variable is referenced here
    2, imgui_widgets.cpp:7418:40: incrementing and referencing a variable in a complex condition can cause unintended side-effects due to C++'s order of evaluation, consider moving the modification outside of the condition to avoid misunderstandings

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:8643:37: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
            const int v1_idx = (int)(t0 * item_count + 0.5f);
                                    ^
  Report hash: 83ebc931af28bf1edd9de5bbdd26d129
  Steps:
    1, imgui_widgets.cpp:8643:37: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:8829:22: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    ImRect clip_rect(IM_ROUND(bar_rect.Min.x + window->WindowBorderSize * 0.5f), IM_ROUND(bar_rect.Min.y + border_top), IM_ROUND(ImMax(bar_rect.Min.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize * 0.5f))), IM_ROUND(bar_rect.Max.y));
                     ^
  Report hash: 4c86ea81e5c705bd5edce109ec8eae35
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:8829:22: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:8829:82: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    ImRect clip_rect(IM_ROUND(bar_rect.Min.x + window->WindowBorderSize * 0.5f), IM_ROUND(bar_rect.Min.y + border_top), IM_ROUND(ImMax(bar_rect.Min.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize * 0.5f))), IM_ROUND(bar_rect.Max.y));
                                                                                 ^
  Report hash: c04c39d2117fad5ed8a1c98d20e6f3e7
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:8829:82: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:8829:121: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    ImRect clip_rect(IM_ROUND(bar_rect.Min.x + window->WindowBorderSize * 0.5f), IM_ROUND(bar_rect.Min.y + border_top), IM_ROUND(ImMax(bar_rect.Min.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize * 0.5f))), IM_ROUND(bar_rect.Max.y));
                                                                                                                        ^
  Report hash: 3cce5d1eceb5591334d597f00a9c53db
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:8829:121: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[HIGH] /sep/third_party/imgui/imgui_widgets.cpp:8829:235: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
    ImRect clip_rect(IM_ROUND(bar_rect.Min.x + window->WindowBorderSize * 0.5f), IM_ROUND(bar_rect.Min.y + border_top), IM_ROUND(ImMax(bar_rect.Min.x, bar_rect.Max.x - ImMax(window->WindowRounding, window->WindowBorderSize * 0.5f))), IM_ROUND(bar_rect.Max.y));
                                                                                                                                                                                                                                          ^
  Report hash: 2eaaa1af3b86ab6f68bbc3c1ba82f366
  Steps:
    1, imgui_internal.h:278:54: expanded from macro 'IM_ROUND'
    2, imgui_widgets.cpp:8829:235: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

[LOW] /sep/third_party/imgui/imgui_widgets.cpp:9066:19: 2nd argument 'menu_is_open' (passed to 'selected') looks like it might be swapped with the 3rd, 'selectable_flags' (passed to 'flags') [readability-suspicious-call-argument]
        pressed = Selectable("", menu_is_open, selectable_flags, ImVec2(w, label_size.y));
                  ^
  Report hash: cb093e27d0deb7e5e82179876cc46267
  Steps:
    1, imgui_widgets.cpp:7116:13: in the call to 'Selectable', declared here
    2, imgui_widgets.cpp:9066:19: 2nd argument 'menu_is_open' (passed to 'selected') looks like it might be swapped with the 3rd, 'selectable_flags' (passed to 'flags')

[MEDIUM] /sep/third_party/imgui/imgui_widgets.cpp:9379:5: undefined behavior, destination object type 'ImGuiTabBar' is not TriviallyCopyable [bugprone-undefined-memory-manipulation]
    memset(this, 0, sizeof(*this));
    ^
  Report hash: 8af15ddce26e2973d81a0fcff858ee53
  Steps:
    1, imgui_widgets.cpp:9379:5: undefined behavior, destination object type 'ImGuiTabBar' is not TriviallyCopyable

Found 38 defect(s) in imgui_widgets.cpp

[MEDIUM] /sep/src/core/config_manager_stub.cpp:5:1: 'Impl' defined as a class here but previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
class ConfigManager::Impl {
^
  Report hash: a52f1bed6757a6bdcae97e461dc010a2
  Steps:
    1, manager.h:66:3: did you mean class here?
    2, config_manager_stub.cpp:5:1: 'Impl' defined as a class here but previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 1 defect(s) in config_manager_stub.cpp

[LOW] /sep/src/sep_engine_wrapper.h:24:12: no definition found for 'Pattern', but a definition with the same name 'Pattern' found in another namespace 'sep::workbench' [bugprone-forward-declaration-namespace]
    struct Pattern;
           ^
  Report hash: 8fd89ed7ef03fa39831a248e4b15a029
  Steps:
    1, pattern.hpp:13:8: a definition of 'Pattern' is found here
    2, sep_engine_wrapper.h:24:12: no definition found for 'Pattern', but a definition with the same name 'Pattern' found in another namespace 'sep::workbench'

Found 1 defect(s) in sep_engine_wrapper.h

[MEDIUM] /sep/src/workbench_main.cpp:179:29: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                            ^
  Report hash: 1524651e4d75aed58f3b8d06533ba36a
  Notes:
    1, workbench_main.cpp:179:9: ~~~~~ (fixit)
    2, workbench_main.cpp:179:21: 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); (fixit)
    3, workbench_main.cpp:179:21: ~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~ (fixit)
  Steps:
    1, workbench_main.cpp:179:29: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench_main.cpp:179:72: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                                                                       ^
  Report hash: f170c7cf085694fde678a0b62362ec54
  Notes:
    1, workbench_main.cpp:179:9: ~~~~~ (fixit)
    2, workbench_main.cpp:179:21: 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate); (fixit)
    3, workbench_main.cpp:179:57: ~~~~~~~~~~~~~~~^~~~~~~~~ (fixit)
  Steps:
    1, workbench_main.cpp:179:72: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in workbench_main.cpp

Found no defects in server.cpp
[MEDIUM] /sep/src/compat/memory.h:112:63: unused parameter 'stream' [clang-diagnostic-unused-parameter]
bool copyToDevice(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
                                                              ^
  Report hash: 03fe062d6c7e2fcf37a2b293d4c6d7a1
  Steps:
    1, memory.h:112:63: unused parameter 'stream'

[MEDIUM] /sep/src/compat/memory.h:124:61: unused parameter 'stream' [clang-diagnostic-unused-parameter]
bool copyToHost(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
                                                            ^
  Report hash: d7af2a21b214b2470caa64843624d8f9
  Steps:
    1, memory.h:124:61: unused parameter 'stream'

Found 2 defect(s) in memory.h

[MEDIUM] /sep/src/core/engine.cpp:88:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  fflush(stdout);
  ^
  Report hash: 6e12c4fd64fc76995ebc3b185ba355a3
  Steps:
    1, engine.cpp:88:3: cast the expression to void to silence this warning
    2, engine.cpp:88:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:93:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  fflush(stdout);
  ^
  Report hash: 6e12c4fd64fc76995ebc3b185ba355a3
  Steps:
    1, engine.cpp:93:3: cast the expression to void to silence this warning
    2, engine.cpp:93:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in engine.cpp

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:367:31: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
                   out_block->coherence, out_block->stability);
                              ^
  Report hash: 37e385ab7388829d43ef552db7143b61
  Notes:
    1, memory_tier_manager.cpp:367:13: ~~~~~~ (fixit)
    2, memory_tier_manager.cpp:367:20: out_block->coherence, out_block->stability); (fixit)
    3, memory_tier_manager.cpp:367:20: ~~~~~~~~~~~^~~~~~~~~ (fixit)
  Steps:
    1, memory_tier_manager.cpp:367:31: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:367:53: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
                   out_block->coherence, out_block->stability);
                                                    ^
  Report hash: 9506b31759ee71ddc2d04f59ed8c691e
  Notes:
    1, memory_tier_manager.cpp:367:13: ~~~~~~ (fixit)
    2, memory_tier_manager.cpp:367:20: out_block->coherence, out_block->stability); (fixit)
    3, memory_tier_manager.cpp:367:42: ~~~~~~~~~~~^~~~~~~~~ (fixit)
  Steps:
    1, memory_tier_manager.cpp:367:53: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in memory_tier_manager.cpp

[MEDIUM] /sep/src/quantum/evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        state.phase += (nextFloat() * 2.0f - 1.0f) * sigma * M_PI; // Add phase mutation
                                                   ^
  Report hash: c5677e825a782969782849e7a3b87e33
  Steps:
    1, evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:324:42: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    state.phase += (rnd() * 2.0f - 1.0f) * sigma * M_PI; // Add phase mutation
                                         ^
  Report hash: cad84b4fd50cd0e5a22044d2f4918545
  Steps:
    1, evolution.cpp:324:42: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:376:45: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        pattern.quantum_state.phase = rnd() * 2.0f * M_PI; // Initialize phase
                                            ^
  Report hash: 686c468a0c599aae810685813b0a5284
  Steps:
    1, evolution.cpp:376:45: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in evolution.cpp

[LOW] /sep/src/audio/pipewire_capture.cpp:320:13: Call to blocking function 'fgets' inside of critical section [unix.BlockInCriticalSection]
        if (fgets(service_status, sizeof(service_status), fp) != nullptr) {
            ^
  Report hash: 616432282e756e15fcd5f97426498279
  Macro expansions:
    1, gthr-default.h:791:1: Macro '__GTHREAD_INLINE' expanded to 'inline __attribute__ ((__always_inline__ ))'
    2, gthr-default.h:795:12: Macro '__gthrw_(pthread_mutex_lock)' expanded to 'pthread_mutex_lock '
  Steps:
     1, pipewire_capture.cpp:208:33: Calling constructor for 'lock_guard<std::mutex>'
     2, std_mutex.h:251:7: Entered call from 'PipeWireCapture::init'
     3, std_mutex.h:252:9: Calling 'mutex::lock'
     4, std_mutex.h:112:5: Entered call from constructor for 'lock_guard<std::mutex>'
     5, std_mutex.h:115:17: Calling '__gthread_mutex_lock'
     6, gthr-default.h:791:1: Entered call from 'mutex::lock'
     7, gthr-default.h:795:12: Entering critical section here
     8, std_mutex.h:115:17: Returning from '__gthread_mutex_lock'
     9, std_mutex.h:118:11: Assuming '__e' is 0
    10, std_mutex.h:252:9: Returning from 'mutex::lock'
    11, std_mutex.h:252:9: Entering critical section here
    12, pipewire_capture.cpp:208:33: Returning from constructor for 'lock_guard<std::mutex>'
    13, pipewire_capture.cpp:208:33: Entering critical section here
    14, pipewire_capture.cpp:211:9: Assuming field 'ok' is true
    15, pipewire_capture.cpp:237:9: Assuming field 'loop_' is non-null
    16, pipewire_capture.cpp:246:9: Assuming 'start_err' is >= 0
    17, pipewire_capture.cpp:269:9: Assuming field 'context_' is non-null
    18, pipewire_capture.cpp:287:9: Assuming the condition is false
    19, pipewire_capture.cpp:301:30: Entering loop body
    20, pipewire_capture.cpp:304:13: Assuming the condition is false
    21, pipewire_capture.cpp:320:13: Call to blocking function 'fgets' inside of critical section

[LOW] /sep/src/audio/pipewire_capture.cpp:338:28: Call to blocking function 'fgets' inside of critical section [unix.BlockInCriticalSection]
                    while (fgets(status_buf, sizeof(status_buf), status_fp)) {
                           ^
  Report hash: 0a4eea9e7ffe17ffd7a730f13b29114e
  Macro expansions:
    1, gthr-default.h:791:1: Macro '__GTHREAD_INLINE' expanded to 'inline __attribute__ ((__always_inline__ ))'
    2, gthr-default.h:795:12: Macro '__gthrw_(pthread_mutex_lock)' expanded to 'pthread_mutex_lock '
  Steps:
     1, pipewire_capture.cpp:208:33: Calling constructor for 'lock_guard<std::mutex>'
     2, std_mutex.h:251:7: Entered call from 'PipeWireCapture::init'
     3, std_mutex.h:252:9: Calling 'mutex::lock'
     4, std_mutex.h:112:5: Entered call from constructor for 'lock_guard<std::mutex>'
     5, std_mutex.h:115:17: Calling '__gthread_mutex_lock'
     6, gthr-default.h:791:1: Entered call from 'mutex::lock'
     7, gthr-default.h:795:12: Entering critical section here
     8, std_mutex.h:115:17: Returning from '__gthread_mutex_lock'
     9, std_mutex.h:118:11: Assuming '__e' is 0
    10, std_mutex.h:252:9: Returning from 'mutex::lock'
    11, std_mutex.h:252:9: Entering critical section here
    12, pipewire_capture.cpp:208:33: Returning from constructor for 'lock_guard<std::mutex>'
    13, pipewire_capture.cpp:208:33: Entering critical section here
    14, pipewire_capture.cpp:211:9: Assuming field 'ok' is true
    15, pipewire_capture.cpp:237:9: Assuming field 'loop_' is non-null
    16, pipewire_capture.cpp:246:9: Assuming 'start_err' is >= 0
    17, pipewire_capture.cpp:269:9: Assuming field 'context_' is non-null
    18, pipewire_capture.cpp:287:9: Assuming the condition is false
    19, pipewire_capture.cpp:301:30: Entering loop body
    20, pipewire_capture.cpp:304:13: Assuming the condition is false
    21, pipewire_capture.cpp:331:17: Assuming 'start_result' is not equal to 0
    22, pipewire_capture.cpp:338:28: Call to blocking function 'fgets' inside of critical section

[LOW] /sep/src/audio/pipewire_capture.cpp:357:25: Call to blocking function 'fgets' inside of critical section [unix.BlockInCriticalSection]
                    if (fgets(service_status, sizeof(service_status), check_fp) != nullptr) {
                        ^
  Report hash: 2598cca41cc564f89858bd7d96772cd7
  Macro expansions:
    1, gthr-default.h:791:1: Macro '__GTHREAD_INLINE' expanded to 'inline __attribute__ ((__always_inline__ ))'
    2, gthr-default.h:795:12: Macro '__gthrw_(pthread_mutex_lock)' expanded to 'pthread_mutex_lock '
  Steps:
     1, pipewire_capture.cpp:208:33: Calling constructor for 'lock_guard<std::mutex>'
     2, std_mutex.h:251:7: Entered call from 'PipeWireCapture::init'
     3, std_mutex.h:252:9: Calling 'mutex::lock'
     4, std_mutex.h:112:5: Entered call from constructor for 'lock_guard<std::mutex>'
     5, std_mutex.h:115:17: Calling '__gthread_mutex_lock'
     6, gthr-default.h:791:1: Entered call from 'mutex::lock'
     7, gthr-default.h:795:12: Entering critical section here
     8, std_mutex.h:115:17: Returning from '__gthread_mutex_lock'
     9, std_mutex.h:118:11: Assuming '__e' is 0
    10, std_mutex.h:252:9: Returning from 'mutex::lock'
    11, std_mutex.h:252:9: Entering critical section here
    12, pipewire_capture.cpp:208:33: Returning from constructor for 'lock_guard<std::mutex>'
    13, pipewire_capture.cpp:208:33: Entering critical section here
    14, pipewire_capture.cpp:211:9: Assuming field 'ok' is true
    15, pipewire_capture.cpp:237:9: Assuming field 'loop_' is non-null
    16, pipewire_capture.cpp:246:9: Assuming 'start_err' is >= 0
    17, pipewire_capture.cpp:269:9: Assuming field 'context_' is non-null
    18, pipewire_capture.cpp:287:9: Assuming the condition is false
    19, pipewire_capture.cpp:301:30: Entering loop body
    20, pipewire_capture.cpp:304:13: Assuming the condition is false
    21, pipewire_capture.cpp:331:17: Assuming 'start_result' is equal to 0
    22, pipewire_capture.cpp:352:20: Entering loop body
    23, pipewire_capture.cpp:357:25: Call to blocking function 'fgets' inside of critical section

Found 3 defect(s) in pipewire_capture.cpp

[LOW] /sep/src/memory/types.h:28:15: no definition found for 'IRedisManager', but a definition with the same name 'IRedisManager' found in another namespace 'sep::persistence' [bugprone-forward-declaration-namespace]
        class IRedisManager;
              ^
  Report hash: bb6220cfbb10b287bf51f1bcfb170272
  Steps:
    1, redis_manager.h:23:7: a definition of 'IRedisManager' is found here
    2, types.h:28:15: no definition found for 'IRedisManager', but a definition with the same name 'IRedisManager' found in another namespace 'sep::persistence'

Found 1 defect(s) in types.h

[MEDIUM] /sep/src/memory/memory_tier.cpp:700:26: std::move of the variable 'pattern' of the trivially-copyable type '::sep::persistence::PersistentPatternData' has no effect; remove std::move() [performance-move-const-arg]
        m_patterns[id] = std::move(pattern);
                         ^
  Report hash: db3b6b6749836b7ff0214ba208ff5162
  Steps:
    1, memory_tier.cpp:700:26: std::move of the variable 'pattern' of the trivially-copyable type '::sep::persistence::PersistentPatternData' has no effect; remove std::move()

Found 1 defect(s) in memory_tier.cpp

Found no defects in factory.cpp
[LOW] /usr/include/pipewire-0.3/pipewire/control.h:25:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: fd8eadb9bf09754217dc928cc66a2a3f
  Steps:
    1, impl.h:24:10: 'control.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, control.h:25:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in control.h

[LOW] /usr/include/pipewire-0.3/pipewire/core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 1daf003ee682828fd0bcf5e77b8a5137
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, protocol.h:26:10: 'context.h' included from here
    3, core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path

[LOW] /usr/include/pipewire-0.3/pipewire/core.h:79:10: circular header file dependency detected while including 'proxy.h', please check the include path [misc-header-include-cycle]
#include <pipewire/proxy.h>
         ^
  Report hash: 82346608a24564d39352cc5913ffbb82
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, protocol.h:26:10: 'context.h' included from here
    3, proxy.h:90:10: 'protocol.h' included from here
    4, client.h:16:10: 'proxy.h' included from here
    5, core.h:79:10: circular header file dependency detected while including 'proxy.h', please check the include path

[MEDIUM] /usr/include/pipewire-0.3/pipewire/core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  vsnprintf(buffer, sizeof(buffer), message, args);
  ^
  Report hash: 0ed97ae05ae09f99abd5bfafd2bde595
  Steps:
    1, core.h:396:2: cast the expression to void to silence this warning
    2, core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 3 defect(s) in core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: c36587793019ad5530065d0f43f044ca
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl.h:25:10: 'impl-core.h' included from here
    4, global.h:39:10: 'impl.h' included from here
    5, pipewire_includes.h:17:10: 'global.h' included from here
    6, impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path

[LOW] /usr/include/pipewire-0.3/pipewire/impl-client.h:68:10: circular header file dependency detected while including 'resource.h', please check the include path [misc-header-include-cycle]
#include <pipewire/resource.h>
         ^
  Report hash: 0dbd6e13aed46b4d8732e99ad49ef4c1
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl-client.h:68:10: circular header file dependency detected while including 'resource.h', please check the include path

Found 2 defect(s) in impl-client.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: b4e1d1024c73f5543ef983da74c8a645
  Steps:
    1, impl.h:25:10: 'impl-core.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:17:10: 'global.h' included from here
    4, impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 98baed36db9cbd1ca48fe18b801bea5f
  Steps:
    1, impl.h:27:10: 'impl-device.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:17:10: 'global.h' included from here
    4, impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-device.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: caa7dc7877996ab29c3b85d029b375c4
  Steps:
    1, impl.h:28:10: 'impl-factory.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:17:10: 'global.h' included from here
    4, impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-factory.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-link.h:24:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: 89e4605111d4c286b81e1b516e7160e7
  Steps:
    1, impl.h:30:10: 'impl-link.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-link.h:24:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-link.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 49976cb41585c3805dce471218afb328
  Steps:
    1, impl.h:31:10: 'impl-metadata.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:17:10: 'global.h' included from here
    4, impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-metadata.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-node.h:28:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: b556a94963dd35d190a0ddd6ee0964b5
  Steps:
    1, impl.h:33:10: 'impl-node.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-node.h:28:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-node.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-port.h:27:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: 3a0828555eb26fb9f898376dc802be93
  Steps:
    1, impl.h:34:10: 'impl-port.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-port.h:27:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-port.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 97378dfee83bf376433e4226817d4af1
  Steps:
    1, global.h:39:10: 'impl.h' included from here
    2, pipewire_includes.h:17:10: 'global.h' included from here
    3, impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl.h

[MEDIUM] /usr/include/pipewire-0.3/pipewire/properties.h:192:1: identifier '_spa_autoptr_cleanup_func_pw_properties' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(pw_properties, struct pw_properties, {
^
  Report hash: 83cc49725b0040adc82f792183801b60
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, properties.h:192:1: identifier '_spa_autoptr_cleanup_func_pw_properties' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/pipewire-0.3/pipewire/properties.h:192:1: identifier '_spa_autoptr_cleanup_type_pw_properties' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(pw_properties, struct pw_properties, {
^
  Report hash: 70f2a3d989b98e27528a2b459ef264c4
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, properties.h:192:1: identifier '_spa_autoptr_cleanup_type_pw_properties' is reserved because it starts with '_' at global scope

Found 2 defect(s) in properties.h

[MEDIUM] /usr/include/pipewire-0.3/pipewire/utils.h:103:1: identifier '_spa_auto_cleanup_func_pw_strv' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTO_CLEANUP(pw_strv, char **, {
^
  Report hash: d191108c2492e721db5c198011c6f841
  Steps:
    1, cleanup.h:55:20: expanded from macro 'SPA_DEFINE_AUTO_CLEANUP'
    2, utils.h:103:1: identifier '_spa_auto_cleanup_func_pw_strv' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/pipewire-0.3/pipewire/utils.h:103:1: identifier '_spa_auto_cleanup_type_pw_strv' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTO_CLEANUP(pw_strv, char **, {
^
  Report hash: 286ad65c4b8b98a07728f808e2c0f0cb
  Steps:
    1, cleanup.h:54:26: expanded from macro 'SPA_DEFINE_AUTO_CLEANUP'
    2, utils.h:103:1: identifier '_spa_auto_cleanup_type_pw_strv' is reserved because it starts with '_' at global scope

Found 2 defect(s) in utils.h

[MEDIUM] /usr/include/spa-0.2/spa/buffer/buffer.h:48:2: identifier '_SPA_DATA_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_DATA_LAST,      /**< not part of ABI */
  ^
  Report hash: 03818b597bd5e4204b621d81e5b69bb4
  Steps:
    1, buffer.h:48:2: identifier '_SPA_DATA_LAST' is reserved because it starts with '_' followed by a capital letter

Found 1 defect(s) in buffer.h

[MEDIUM] /usr/include/spa-0.2/spa/buffer/meta.h:41:2: identifier '_SPA_META_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_META_LAST,      /**< not part of ABI/API */
  ^
  Report hash: fb9bba17242cbd932ec11f23751b377d
  Steps:
    1, meta.h:41:2: identifier '_SPA_META_LAST' is reserved because it starts with '_' followed by a capital letter

Found 1 defect(s) in meta.h

[LOW] /usr/include/spa-0.2/spa/param/audio/format-utils.h:56:2: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
  switch (info->media_subtype) {
  ^
  Report hash: ffd5b788737352c33aa8985ddfe3e707
  Steps:
    1, format-utils.h:56:2: switching on non-enum value without default case may not cover all cases

[LOW] /usr/include/spa-0.2/spa/param/audio/format-utils.h:91:2: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
  switch (info->media_subtype) {
  ^
  Report hash: ffd5b788737352c33aa8985ddfe3e707
  Steps:
    1, format-utils.h:91:2: switching on non-enum value without default case may not cover all cases

Found 2 defect(s) in format-utils.h

[LOW] /usr/include/spa-0.2/spa/param/buffers.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: 10f6ec61bd6df1a119cf9eea7e68c8d3
  Steps:
    1, param.h:75:10: 'buffers.h' included from here
    2, client.h:13:10: 'param.h' included from here
    3, buffers.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in buffers.h

[LOW] /usr/include/spa-0.2/spa/param/port-config.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: bbf25c04cf6ba9c76091dc26a9c4907c
  Steps:
    1, param.h:77:10: 'port-config.h' included from here
    2, client.h:13:10: 'param.h' included from here
    3, port-config.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in port-config.h

[LOW] /usr/include/spa-0.2/spa/param/profile.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: d543dd0e6a4219f2f281b6f8d2ec2327
  Steps:
    1, param.h:76:10: 'profile.h' included from here
    2, client.h:13:10: 'param.h' included from here
    3, profile.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in profile.h

[LOW] /usr/include/spa-0.2/spa/param/route.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: de3e7bf38f149ce3fcc8ff8ce5654d20
  Steps:
    1, param.h:78:10: 'route.h' included from here
    2, client.h:13:10: 'param.h' included from here
    3, route.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in route.h

[HIGH] /usr/include/spa-0.2/spa/pod/builder.h:205:3: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
    SPA_FLAG_CLEAR(builder->state.flags, SPA_POD_BUILDER_FLAG_FIRST);
    ^
  Report hash: eef99322fff6dd6b95bd63cedded29aa
  Steps:
    1, defs.h:86:26: expanded from macro 'SPA_FLAG_CLEAR'
    2, builder.h:205:3: suspicious usage of 'sizeof(K)'; did you mean 'K'?

[HIGH] /usr/include/spa-0.2/spa/pod/builder.h:227:2: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
  SPA_FLAG_CLEAR(builder->state.flags, SPA_POD_BUILDER_FLAG_FIRST);
  ^
  Report hash: 8663eacab3b0ba74521abfbc64e40a60
  Steps:
    1, defs.h:86:26: expanded from macro 'SPA_FLAG_CLEAR'
    2, builder.h:227:2: suspicious usage of 'sizeof(K)'; did you mean 'K'?

[LOW] /usr/include/spa-0.2/spa/pod/builder.h:653:4: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      SPA_POD_BUILDER_COLLECT(builder, *format, args);
      ^
  Report hash: 3f893bfb6eace68e3d86b6cf62666a25
  Steps:
    1, builder.h:512:2: expanded from macro 'SPA_POD_BUILDER_COLLECT'
    2, builder.h:653:4: switching on non-enum value without default case may not cover all cases

Found 3 defect(s) in builder.h

[MEDIUM] /usr/include/spa-0.2/spa/pod/iter.h:314:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_return_val_if_fail(spa_pod_is_fraction(pod), -EINVAL);
  ^
  Report hash: 9600fcd5fbb15cb4c336c03fb2e7cc60
  Steps:
    1, defs.h:381:4: expanded from macro 'spa_return_val_if_fail'
    2, iter.h:314:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/pod/iter.h:333:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_return_val_if_fail(spa_pod_is_array(pod), NULL);
  ^
  Report hash: 329b2d035b569160577af71457d8348d
  Steps:
    1, defs.h:381:4: expanded from macro 'spa_return_val_if_fail'
    2, iter.h:333:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in iter.h

[LOW] /usr/include/spa-0.2/spa/pod/parser.h:494:4: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      SPA_POD_PARSER_SKIP(*format, args);
      ^
  Report hash: 936209cc6ea38b2b5f94af2eb67b6ece
  Steps:
    1, parser.h:421:2: expanded from macro 'SPA_POD_PARSER_SKIP'
    2, parser.h:494:4: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in parser.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:83:20: identifier '_spa_autofree_cleanup_func' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
static inline void _spa_autofree_cleanup_func(void *p)
                   ^
  Report hash: 4e9214c45124f4846bac779aaf0f11b8
  Steps:
    1, cleanup.h:83:20: identifier '_spa_autofree_cleanup_func' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:93:20: identifier '_spa_autoclose_cleanup_func' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
static inline void _spa_autoclose_cleanup_func(int *fd)
                   ^
  Report hash: 062d1e75a6f46d35b6f1ed776d69a0bf
  Steps:
    1, cleanup.h:93:20: identifier '_spa_autoclose_cleanup_func' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:105:1: identifier '_spa_autoptr_cleanup_func_FILE' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(FILE, FILE, {
^
  Report hash: 8c90e8714ce36cbfc016a34d6aafe59b
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:105:1: identifier '_spa_autoptr_cleanup_func_FILE' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:105:1: identifier '_spa_autoptr_cleanup_type_FILE' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(FILE, FILE, {
^
  Report hash: 1948d70ed31817f9218c89cacf394cc2
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:105:1: identifier '_spa_autoptr_cleanup_type_FILE' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:106:24: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_clear_ptr(*thing, fclose);
                        ^
  Report hash: 6dc3766dd4a30ea03f6b48247051ef9d
  Steps:
    1, cleanup.h:28:3: expanded from macro 'spa_clear_ptr'
    2, cleanup.h:71:2: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    3, cleanup.h:106:24: cast the expression to void to silence this warning
    4, cleanup.h:106:24: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:113:1: identifier '_spa_autoptr_cleanup_func_DIR' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(DIR, DIR, {
^
  Report hash: e974bef354d1e377f8e0bba41689668c
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:113:1: identifier '_spa_autoptr_cleanup_func_DIR' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:113:1: identifier '_spa_autoptr_cleanup_type_DIR' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(DIR, DIR, {
^
  Report hash: 250ac9d4570be92c586f490e4104b572
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:113:1: identifier '_spa_autoptr_cleanup_type_DIR' is reserved because it starts with '_' at global scope

Found 7 defect(s) in cleanup.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:3: identifier '__NONE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
    ^
  Report hash: e169b04a5693dda3263a71a92be29757
  Steps:
    1, json-core.h:83:3: identifier '__NONE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:11: identifier '__STRUCT' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
            ^
  Report hash: a686f6b9af1c086abced146c0c6f2435
  Steps:
    1, json-core.h:83:11: identifier '__STRUCT' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:21: identifier '__BARE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                      ^
  Report hash: c36fb0b6ba22ab12a23f29403fa42dc0
  Steps:
    1, json-core.h:83:21: identifier '__BARE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:29: identifier '__STRING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                              ^
  Report hash: a32457a4f1b73b65aac2a15b621291f6
  Steps:
    1, json-core.h:83:29: identifier '__STRING' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:39: identifier '__UTF8' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                        ^
  Report hash: ff655471788b4ed203ea6836ce8d6ffa
  Steps:
    1, json-core.h:83:39: identifier '__UTF8' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:47: identifier '__ESC' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                                ^
  Report hash: 222e64a766587807525f5bfdb554e6c4
  Steps:
    1, json-core.h:83:47: identifier '__ESC' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:54: identifier '__COMMENT' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                                       ^
  Report hash: 5034e9e0efdb387c2993b905a5c6d2ee
  Steps:
    1, json-core.h:83:54: identifier '__COMMENT' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:84:3: identifier '__ARRAY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ARRAY_FLAG = 0x10,    /* in array context */
    ^
  Report hash: df193b651db2bc004cfce54a372ee4bf
  Steps:
    1, json-core.h:84:3: identifier '__ARRAY_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:85:3: identifier '__PREV_ARRAY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __PREV_ARRAY_FLAG = 0x20,  /* depth=0 array context flag */
    ^
  Report hash: ea668d02342d7cdd6119ca7e34f58ee0
  Steps:
    1, json-core.h:85:3: identifier '__PREV_ARRAY_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:86:3: identifier '__KEY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __KEY_FLAG = 0x40,    /* inside object key */
    ^
  Report hash: 93a8368e3478ff6d782b10059d0bf152
  Steps:
    1, json-core.h:86:3: identifier '__KEY_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:87:3: identifier '__SUB_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __SUB_FLAG = 0x80,    /* not at top-level */
    ^
  Report hash: f83d92cb8f26170b1b651b4cc5257349
  Steps:
    1, json-core.h:87:3: identifier '__SUB_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:88:3: identifier '__FLAGS' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __FLAGS = 0xff0,
    ^
  Report hash: 8a480e1edd6a9ea99102e0062640b75d
  Steps:
    1, json-core.h:88:3: identifier '__FLAGS' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:89:3: identifier '__ERROR_SYSTEM' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_SYSTEM = SPA_JSON_ERROR_FLAG,
    ^
  Report hash: 63fc14343b21c8b5954c7a6f23d09d13
  Steps:
    1, json-core.h:89:3: identifier '__ERROR_SYSTEM' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:90:3: identifier '__ERROR_INVALID_ARRAY_SEPARATOR' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_ARRAY_SEPARATOR,
    ^
  Report hash: 90e5106128638f8e72fcf1dd0ff5d7eb
  Steps:
    1, json-core.h:90:3: identifier '__ERROR_INVALID_ARRAY_SEPARATOR' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:91:3: identifier '__ERROR_EXPECTED_OBJECT_KEY' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_KEY,
    ^
  Report hash: 0eeebef6a1a296298259434bd9c83b80
  Steps:
    1, json-core.h:91:3: identifier '__ERROR_EXPECTED_OBJECT_KEY' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:92:3: identifier '__ERROR_EXPECTED_OBJECT_VALUE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_VALUE,
    ^
  Report hash: 3701429a89c1e0ae596ec4920c380361
  Steps:
    1, json-core.h:92:3: identifier '__ERROR_EXPECTED_OBJECT_VALUE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:93:3: identifier '__ERROR_TOO_DEEP_NESTING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_TOO_DEEP_NESTING,
    ^
  Report hash: edd9a0657db2889b4a6fa1d8926be5f2
  Steps:
    1, json-core.h:93:3: identifier '__ERROR_TOO_DEEP_NESTING' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:94:3: identifier '__ERROR_EXPECTED_ARRAY_CLOSE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_ARRAY_CLOSE,
    ^
  Report hash: 86c42b9d2c3d9b54aab616415c4389e5
  Steps:
    1, json-core.h:94:3: identifier '__ERROR_EXPECTED_ARRAY_CLOSE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:95:3: identifier '__ERROR_EXPECTED_OBJECT_CLOSE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_CLOSE,
    ^
  Report hash: a20150bb32b72233751ff8c5e718fc0c
  Steps:
    1, json-core.h:95:3: identifier '__ERROR_EXPECTED_OBJECT_CLOSE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:96:3: identifier '__ERROR_MISMATCHED_BRACKET' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_MISMATCHED_BRACKET,
    ^
  Report hash: 18220c4cdc2c9b88054ae2dc3acfeab3
  Steps:
    1, json-core.h:96:3: identifier '__ERROR_MISMATCHED_BRACKET' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:97:3: identifier '__ERROR_ESCAPE_NOT_ALLOWED' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_ESCAPE_NOT_ALLOWED,
    ^
  Report hash: ed3b1e7cb46d2343adab35416dba18e7
  Steps:
    1, json-core.h:97:3: identifier '__ERROR_ESCAPE_NOT_ALLOWED' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:98:3: identifier '__ERROR_CHARACTERS_NOT_ALLOWED' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_CHARACTERS_NOT_ALLOWED,
    ^
  Report hash: 3011991984ae22eb59322ec4b2c64dd7
  Steps:
    1, json-core.h:98:3: identifier '__ERROR_CHARACTERS_NOT_ALLOWED' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:99:3: identifier '__ERROR_INVALID_ESCAPE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_ESCAPE,
    ^
  Report hash: b2d68b5a712901be597bf7b2dadf1427
  Steps:
    1, json-core.h:99:3: identifier '__ERROR_INVALID_ESCAPE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:100:3: identifier '__ERROR_INVALID_STATE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_STATE,
    ^
  Report hash: 34dc1dc7c59fe0e25e45f1c9f7faf58d
  Steps:
    1, json-core.h:100:3: identifier '__ERROR_INVALID_STATE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:101:3: identifier '__ERROR_UNFINISHED_STRING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_UNFINISHED_STRING,
    ^
  Report hash: e36b85e610b8c7631cc4c58cc016c648
  Steps:
    1, json-core.h:101:3: identifier '__ERROR_UNFINISHED_STRING' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:114:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _SPA_ERROR(reason)  { err = __ERROR_ ## reason; goto error; }
        ^
  Report hash: 901bfff7582981f1d0da07bbef233ed0
  Steps:
    1, json-core.h:114:9: macro name is a reserved identifier

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:323:8: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#undef _SPA_ERROR
       ^
  Report hash: 0bb09aa6989530fb260dc1931e26bbb9
  Steps:
    1, json-core.h:323:8: macro name is a reserved identifier

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:449:29: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
  return spa_dtoa(str, size, val);
                             ^
  Report hash: 57f4ffa6f2535055a551be4e2940a10c
  Steps:
    1, json-core.h:449:29: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:595:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __PUT(c) { if (len < size) *str++ = c; len++; }
        ^
  Report hash: 34755b98cae0df37310c8f6bcf293d28
  Steps:
    1, json-core.h:595:9: macro name is a reserved identifier

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:632:8: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#undef __PUT
       ^
  Report hash: 42b5c89c010f749d85d0fc0838fd4497
  Steps:
    1, json-core.h:632:8: macro name is a reserved identifier

Found 30 defect(s) in json-core.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/string.h:70:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se(prefix);
  ^
  Report hash: 5019cf126b16580a5adaf138b242121b
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:70:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/utils/string.h:88:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se(suffix);
  ^
  Report hash: 65f20f678659dd9b1b5a8318724c88f0
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:88:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/utils/string.h:225:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se((ssize_t)size > 0);
  ^
  Report hash: d5a84d0cea4cae9623806e567493b935
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:225:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 3 defect(s) in string.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:55:2: identifier '_SPA_TYPE_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_LAST,        /**< not part of ABI */
  ^
  Report hash: bdb7800d6f84cfde632ca52fb2f44eab
  Steps:
    1, type.h:55:2: identifier '_SPA_TYPE_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:62:2: identifier '_SPA_TYPE_POINTER_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_POINTER_LAST,      /**< not part of ABI */
  ^
  Report hash: 1e32f82091e76d3c89ebf1ef8d990e74
  Steps:
    1, type.h:62:2: identifier '_SPA_TYPE_POINTER_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:68:2: identifier '_SPA_TYPE_EVENT_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_EVENT_LAST,      /**< not part of ABI */
  ^
  Report hash: 90102cb980d9dec27ffe550717137a79
  Steps:
    1, type.h:68:2: identifier '_SPA_TYPE_EVENT_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:74:2: identifier '_SPA_TYPE_COMMAND_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_COMMAND_LAST,      /**< not part of ABI */
  ^
  Report hash: 08f54e700f5760bf5f2a4537a0e2a915
  Steps:
    1, type.h:74:2: identifier '_SPA_TYPE_COMMAND_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:91:2: identifier '_SPA_TYPE_OBJECT_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_OBJECT_LAST,      /**< not part of ABI */
  ^
  Report hash: a68bc4cbf6a446cdbcaf462db3488a0a
  Steps:
    1, type.h:91:2: identifier '_SPA_TYPE_OBJECT_LAST' is reserved because it starts with '_' followed by a capital letter

Found 5 defect(s) in type.h

[LOW] /sep/src/crow/common.h:5:10: direct self-inclusion of header file 'common.h' [misc-header-include-cycle]
#include <crow/common.h>
         ^
  Report hash: 4d05be001d84b3afeb8864f287a93dcd
  Steps:
    1, common.h:5:10: direct self-inclusion of header file 'common.h'

Found 1 defect(s) in common.h

[MEDIUM] /sep/src/crow/http_response.h:10:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
    struct response;
    ^
  Report hash: 66dd6ae82c650fb3018a7c958a82ca37
  Steps:
    1, crow_isolation.h:91:11: previous use is here
    2, http_response.h:10:5: did you mean class here?
    3, http_response.h:10:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 1 defect(s) in http_response.h

[MEDIUM] /sep/src/audio/config.cpp:52:31: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                              ^
  Report hash: 4e63877f53b67c4fbc4445ca607d7ea4
  Steps:
    1, config.cpp:52:31: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:52:39: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                      ^
  Report hash: 6512e0d023076ff627be170438437f62
  Steps:
    1, config.cpp:52:39: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:52:66: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                                                 ^
  Report hash: c6d2f61264b32d1de6f0cf1588bf3524
  Steps:
    1, config.cpp:52:66: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:52:113: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                                                                                                ^
  Report hash: e4cd4b34889d40705121d381bb2d262d
  Steps:
    1, config.cpp:52:113: implicit conversion increases floating-point precision: 'float' to 'double'

Found 4 defect(s) in config.cpp

[MEDIUM] /sep/src/api/server.cpp:770:53: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
  if (coherence_score < config_.response_modulation.coherence_threshold) {
                                                    ^
  Report hash: 9d1543cea2f85bd11b627de29ba7cef4
  Steps:
    1, server.cpp:770:53: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/api/server.cpp:796:66: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    {"simplified", coherence_score < config_.response_modulation.coherence_threshold}
                                                                 ^
  Report hash: 540e3e8a871eca2a472d3d3394c2a008
  Steps:
    1, server.cpp:796:66: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in server.cpp

[MEDIUM] /sep/src/api/server.h:222:38: private field 'cycles_renderer_' is not used [clang-diagnostic-unused-private-field]
  sep::blender::ccl::CyclesRenderer* cycles_renderer_;
                                     ^
  Report hash: 3a19fccf9675b2d499ff0269f54d5087
  Steps:
    1, server.h:222:38: private field 'cycles_renderer_' is not used

Found 1 defect(s) in server.h

[MEDIUM] /sep/src/audio/pipewire_capture.cpp:223:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        snprintf(runtime_path, sizeof(runtime_path), "/run/user/%d", uid);
        ^
  Report hash: 0e308df13dda55c08b2861ff936dfe97
  Steps:
    1, pipewire_capture.cpp:223:9: cast the expression to void to silence this warning
    2, pipewire_capture.cpp:223:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in pipewire_capture.cpp

[MEDIUM] /sep/src/audio/pipewire_capture.h:45:21: private field 'pod_builder_' is not used [clang-diagnostic-unused-private-field]
    spa_pod_builder pod_builder_{};
                    ^
  Report hash: d0182a6fb517cfc232d08869a1078af4
  Steps:
    1, pipewire_capture.h:45:21: private field 'pod_builder_' is not used

[MEDIUM] /sep/src/audio/pipewire_capture.h:46:22: private field 'stream_events_' is not used [clang-diagnostic-unused-private-field]
    pw_stream_events stream_events_{};
                     ^
  Report hash: 98f83c942ea1f2cf984aa08ddd1e2213
  Steps:
    1, pipewire_capture.h:46:22: private field 'stream_events_' is not used

Found 2 defect(s) in pipewire_capture.h

[MEDIUM] /sep/src/workbench/demos/annealing_demo.cpp:42:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double') [clang-diagnostic-double-promotion]
            float invDist6 = 1.0f / std::pow(dist, 6);
                             ^
  Report hash: 145ff45a1fe785c34fd9d4a7e29bef4c
  Steps:
    1, annealing_demo.cpp:42:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double')

Found 1 defect(s) in annealing_demo.cpp

Found no defects in bridge_c.cpp
[MEDIUM] /sep/src/crow/socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.close(asio_ec);
            ^
  Report hash: 1629e317d5b05cb928c747742385246e
  Steps:
    1, socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.close(asio_ec);
            ^
  Report hash: 091fa2ff02de86b4be59dbc550785154
  Steps:
    1, socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(tcp::socket::shutdown_both, asio_ec);
            ^
  Report hash: 69612480ceca2420530f07aad8de41dd
  Steps:
    1, socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(tcp::socket::shutdown_both, asio_ec);
            ^
  Report hash: b5222b50184cfb8b08d37b2b99b70afd
  Steps:
    1, socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(tcp::socket::shutdown_send, asio_ec);
            ^
  Report hash: 71fa8b3825baf69b7365148888335ea7
  Steps:
    1, socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(tcp::socket::shutdown_send, asio_ec);
            ^
  Report hash: 0d7b46c030b7a4b4ead511839acc1607
  Steps:
    1, socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(tcp::socket::shutdown_receive, asio_ec);
            ^
  Report hash: 6bb1e574668db4158f049115d4b4b701
  Steps:
    1, socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(tcp::socket::shutdown_receive, asio_ec);
            ^
  Report hash: d4d0033e652761b687708956539b1022
  Steps:
    1, socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 8 defect(s) in socket_adaptors.h

[LOW] /sep/src/workbench/demos/audio_visualizer.cpp:125:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: f1b9a1ded2e0ee7c6494d7541f1eab45
  Steps:
    1, audio_visualizer.cpp:125:5: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.cpp:191:49: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        ImGui::Text("Peak Level: %.2f", metrics.peak_level);
                                                ^
  Report hash: be95577f9221d769cefcb3e2d892781d
  Steps:
    1, audio_visualizer.cpp:191:49: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.cpp:192:48: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        ImGui::Text("RMS Level: %.2f", metrics.rms_level);
                                               ^
  Report hash: e5552fc9e2a5c8d923693ac1bc8d0f74
  Steps:
    1, audio_visualizer.cpp:192:48: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.cpp:193:49: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        ImGui::Text("Latency: %.1f ms", metrics.latency_ms);
                                                ^
  Report hash: 770175534cc586c63f6e818df9e8ab0f
  Steps:
    1, audio_visualizer.cpp:193:49: implicit conversion increases floating-point precision: 'float' to 'double'

Found 4 defect(s) in audio_visualizer.cpp

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.hpp:39:11: private field 'threshold_' is not used [clang-diagnostic-unused-private-field]
    float threshold_{0.5f};
          ^
  Report hash: 1918dc27a25722adf00ab35c35c3eb70
  Steps:
    1, audio_visualizer.hpp:39:11: private field 'threshold_' is not used

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.hpp:40:11: private field 'decay_' is not used [clang-diagnostic-unused-private-field]
    float decay_{0.1f};
          ^
  Report hash: 3e45763c4591d267d4646b864c3574bb
  Steps:
    1, audio_visualizer.hpp:40:11: private field 'decay_' is not used

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.hpp:41:11: private field 'input_strength_' is not used [clang-diagnostic-unused-private-field]
    float input_strength_{0.5f};
          ^
  Report hash: 0be438527845fdfdf5eb956002221bfb
  Steps:
    1, audio_visualizer.hpp:41:11: private field 'input_strength_' is not used

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.hpp:42:11: private field 'learning_rate_' is not used [clang-diagnostic-unused-private-field]
    float learning_rate_{0.05f};
          ^
  Report hash: 2ce8f8626b0291f524d169d2862220f6
  Steps:
    1, audio_visualizer.hpp:42:11: private field 'learning_rate_' is not used

[MEDIUM] /sep/src/workbench/demos/audio_visualizer.hpp:43:11: private field 'connection_prob_' is not used [clang-diagnostic-unused-private-field]
    float connection_prob_{0.3f};
          ^
  Report hash: fcdce57d970fd38c7ec3eef6fb6d8755
  Steps:
    1, audio_visualizer.hpp:43:11: private field 'connection_prob_' is not used

Found 5 defect(s) in audio_visualizer.hpp

[LOW] /sep/src/sep_engine_wrapper.h:24:12: no definition found for 'Pattern', but a definition with the same name 'Pattern' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
    struct Pattern;
           ^
  Report hash: 3c9c5547d1a22c75f1d22887c1846299
  Steps:
    1, types.h:37:8: a definition of 'Pattern' is found here
    2, sep_engine_wrapper.h:24:12: no definition found for 'Pattern', but a definition with the same name 'Pattern' found in another namespace 'sep::quantum'

[LOW] /sep/src/sep_engine_wrapper.h:25:12: no definition found for 'QuantumState', but a definition with the same name 'QuantumState' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
    struct QuantumState;
           ^
  Report hash: fbbd51d2cf290d4086ea307dd907f1fe
  Steps:
    1, types.h:14:8: a definition of 'QuantumState' is found here
    2, sep_engine_wrapper.h:25:12: no definition found for 'QuantumState', but a definition with the same name 'QuantumState' found in another namespace 'sep::quantum'

Found 2 defect(s) in sep_engine_wrapper.h

[MEDIUM] /sep/src/workbench/demos/annealing_sim.cpp:42:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double') [clang-diagnostic-double-promotion]
            float invDist6 = 1.0f / std::pow(dist, 6);
                             ^
  Report hash: b184919ceaf7e8ac15a99df24654ffac
  Steps:
    1, annealing_sim.cpp:42:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double')

Found 1 defect(s) in annealing_sim.cpp

Found no defects in gpu_context.cpp
[LOW] /sep/src/workbench/demos/audio_visualizer_simple.cpp:79:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: 660425c6524ea691df39414a470ea58d
  Steps:
    1, audio_visualizer_simple.cpp:79:5: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in audio_visualizer_simple.cpp

[MEDIUM] /sep/src/blender/mesh_handler.cpp:303:21: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
      area += 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
                    ^
  Report hash: 96149c0ed1849daeb9b92afc0dcb3cc9
  Steps:
    1, mesh_handler.cpp:303:21: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/blender/mesh_handler.cpp:322:14: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    total += std::sqrt(dx * dx + dy * dy + dz * dz);
             ^
  Report hash: 31bfb7cbaa7554f38c92dbc1014c813e
  Steps:
    1, mesh_handler.cpp:322:14: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in mesh_handler.cpp

Found no defects in pattern_visualization_pipeline.cpp
[MEDIUM] /sep/extern/cycles/src/util/hash.h:555:15: 'signed char' to 'uint' (aka 'unsigned int') conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
  while ((c = *str++)) {
              ^
  Report hash: e273fee2a0a290ebd55727a0afef4ea8
  Steps:
    1, hash.h:555:15: 'signed char' to 'uint' (aka 'unsigned int') conversion; consider casting to 'unsigned char' first.

Found 1 defect(s) in hash.h

Found no defects in cycles_renderer.cpp
[MEDIUM] /sep/extern/cycles/third_party/cuew/include/cuew.h:18:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUEW_H__
        ^
  Report hash: c6e928458ea56789038346ec8ae900f7
  Steps:
    1, cuew.h:18:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/third_party/cuew/include/cuew.h:746:16: tagged union has more data members (5) than tags (4)! [bugprone-tagged-union-member-count]
typedef struct CUDA_RESOURCE_DESC_st {
               ^
  Report hash: 533268e39606365a47e52419ce8f314c
  Steps:
    1, cuew.h:746:16: tagged union has more data members (5) than tags (4)!

[MEDIUM] /sep/extern/cycles/third_party/cuew/include/cuew.h:887:16: identifier '_nvrtcProgram' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
typedef struct _nvrtcProgram *nvrtcProgram;
               ^
  Report hash: 86196a030ae58f454fdfda55c452f317
  Steps:
    1, cuew.h:887:16: identifier '_nvrtcProgram' is reserved because it starts with '_' at global scope

Found 3 defect(s) in cuew.h

[MEDIUM] /sep/extern/cycles/third_party/cuew/src/cuew.c:54:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _LIBRARY_FIND_CHECKED(lib, name) \
        ^
  Report hash: 5b2a9015a33d574455355fe916d4031a
  Steps:
    1, cuew.c:54:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/third_party/cuew/src/cuew.c:58:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _LIBRARY_FIND(lib, name) name = (t##name *)dynamic_library_find(lib, #name);
        ^
  Report hash: 4a062cc68dcad4ed60e180c68f01a17b
  Steps:
    1, cuew.c:58:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/third_party/cuew/src/cuew.c:954:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    fprintf(stderr, "CUDA: failed to run compiler to retrieve version");
    ^
  Report hash: 6636e2f8d2d66ea89793722bb04133d6
  Steps:
    1, cuew.c:954:5: cast the expression to void to silence this warning
    2, cuew.c:954:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/cycles/third_party/cuew/src/cuew.c:969:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    fprintf(stderr, "CUDA: failed to find version number in:\n\n%s\n", output);
    ^
  Report hash: 52604579757b76cf0fa48d3c0ff13861
  Steps:
    1, cuew.c:969:5: cast the expression to void to silence this warning
    2, cuew.c:969:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/cycles/third_party/cuew/src/cuew.c:975:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    fprintf(stderr, "CUDA: failed to parse version number from:\n\n%s\n", output);
    ^
  Report hash: 3b2a13ea564bcc889222fc435d140025
  Steps:
    1, cuew.c:975:5: cast the expression to void to silence this warning
    2, cuew.c:975:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 5 defect(s) in cuew.c

[LOW] /sep/src/workbench/demos/memory_garden.cpp:128:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: dd35e159c90e9e1cd5eb82d4b5050f2e
  Steps:
    1, memory_garden.cpp:128:5: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in memory_garden.cpp

[MEDIUM] /sep/src/workbench/demos/memory_garden.hpp:56:11: private field 'connection_prob_' is not used [clang-diagnostic-unused-private-field]
    float connection_prob_{0.3f};
          ^
  Report hash: add4a2ecfd4eda7637d400a0a56aff7e
  Steps:
    1, memory_garden.hpp:56:11: private field 'connection_prob_' is not used

Found 1 defect(s) in memory_garden.hpp

[MEDIUM] /sep/src/workbench/demos/genesis_pattern.cpp:109:52: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    ImGui::Text("Global Coherence: %.3f", metrics_.coherence);
                                                   ^
  Report hash: 4fbf1f09e1231d4a3caaf4c676ae255b
  Steps:
    1, genesis_pattern.cpp:109:52: implicit conversion increases floating-point precision: 'float' to 'double'

[LOW] /sep/src/workbench/demos/genesis_pattern.cpp:143:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: b7476eb6b59fa98c05113c44f2363ec8
  Steps:
    1, genesis_pattern.cpp:143:5: switching on non-enum value without default case may not cover all cases

Found 2 defect(s) in genesis_pattern.cpp

[MEDIUM] /sep/extern/cycles/third_party/hipew/include/hipew.h:18:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __HIPEW_H__
        ^
  Report hash: c3d3a7918a29df56462f3ad13608f2a3
  Steps:
    1, hipew.h:18:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/third_party/hipew/include/hipew.h:901:16: tagged union has more data members (5) than tags (4)! [bugprone-tagged-union-member-count]
typedef struct HIP_RESOURCE_DESC_st {
               ^
  Report hash: 2a1c571fb0bfb5fd3ea214f39cc23a26
  Steps:
    1, hipew.h:901:16: tagged union has more data members (5) than tags (4)!

[MEDIUM] /sep/extern/cycles/third_party/hipew/include/hipew.h:1108:16: identifier '_hiprtcProgram' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
typedef struct _hiprtcProgram* hiprtcProgram;
               ^
  Report hash: 76e4d9748fa6556e7be3030d4a53f960
  Steps:
    1, hipew.h:1108:16: identifier '_hiprtcProgram' is reserved because it starts with '_' at global scope

Found 3 defect(s) in hipew.h

[MEDIUM] /sep/extern/cycles/third_party/hipew/src/hipew.c:601:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
    fprintf(stderr, "HIP: failed to run compiler to retrieve version");
    ^
  Report hash: 1774487f2781d9b104d9c8f80bf0a781
  Steps:
    1, hipew.c:601:5: cast the expression to void to silence this warning
    2, hipew.c:601:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in hipew.c

[MEDIUM] /sep/src/workbench/renderer.cpp:240:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 - 150);
                       ^
  Report hash: db1554823c1ead68b81f11e3c99b893c
  Steps:
    1, renderer.cpp:240:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:240:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 - 150);
                                        ^
  Report hash: 046d06b8faee592154c4d846d1f30593
  Steps:
    1, renderer.cpp:240:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:241:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 - 150);
                       ^
  Report hash: 23a4312501483417bf883e804755efe9
  Steps:
    1, renderer.cpp:241:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:241:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 - 150);
                                        ^
  Report hash: f0843018b54f1021fe96d0414393457d
  Steps:
    1, renderer.cpp:241:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:242:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 + 150);
                       ^
  Report hash: 745eeff88eb8242caf2a420097605131
  Steps:
    1, renderer.cpp:242:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:242:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 + 150);
                                        ^
  Report hash: b4c739ccaf8f6e23240044f0829a717a
  Steps:
    1, renderer.cpp:242:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:243:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 + 150);
                       ^
  Report hash: 97751113cff576d40e3e57f1c43d607c
  Steps:
    1, renderer.cpp:243:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:243:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 + 150);
                                        ^
  Report hash: edd3e7c6fa48b5d99a164cc1c9ca6b82
  Steps:
    1, renderer.cpp:243:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:250:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 - 150);
                       ^
  Report hash: db1554823c1ead68b81f11e3c99b893c
  Steps:
    1, renderer.cpp:250:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:250:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 - 150);
                                        ^
  Report hash: 046d06b8faee592154c4d846d1f30593
  Steps:
    1, renderer.cpp:250:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:251:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 - 150);
                       ^
  Report hash: 23a4312501483417bf883e804755efe9
  Steps:
    1, renderer.cpp:251:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:251:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 - 150);
                                        ^
  Report hash: f0843018b54f1021fe96d0414393457d
  Steps:
    1, renderer.cpp:251:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:252:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 + 150);
                       ^
  Report hash: 745eeff88eb8242caf2a420097605131
  Steps:
    1, renderer.cpp:252:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:252:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 + 150);
                                        ^
  Report hash: b4c739ccaf8f6e23240044f0829a717a
  Steps:
    1, renderer.cpp:252:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:253:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 + 150);
                       ^
  Report hash: 97751113cff576d40e3e57f1c43d607c
  Steps:
    1, renderer.cpp:253:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:253:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 + 150);
                                        ^
  Report hash: edd3e7c6fa48b5d99a164cc1c9ca6b82
  Steps:
    1, renderer.cpp:253:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:260:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 - 150);
                       ^
  Report hash: db1554823c1ead68b81f11e3c99b893c
  Steps:
    1, renderer.cpp:260:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:260:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 - 150);
                                        ^
  Report hash: 046d06b8faee592154c4d846d1f30593
  Steps:
    1, renderer.cpp:260:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:261:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 + 150);
                       ^
  Report hash: 745eeff88eb8242caf2a420097605131
  Steps:
    1, renderer.cpp:261:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:261:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 + 150);
                                        ^
  Report hash: b4c739ccaf8f6e23240044f0829a717a
  Steps:
    1, renderer.cpp:261:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:262:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 + 150);
                       ^
  Report hash: 97751113cff576d40e3e57f1c43d607c
  Steps:
    1, renderer.cpp:262:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:262:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 150, height / 2 + 150);
                                        ^
  Report hash: edd3e7c6fa48b5d99a164cc1c9ca6b82
  Steps:
    1, renderer.cpp:262:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:263:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 - 150);
                       ^
  Report hash: 23a4312501483417bf883e804755efe9
  Steps:
    1, renderer.cpp:263:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:263:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 150, height / 2 - 150);
                                        ^
  Report hash: f0843018b54f1021fe96d0414393457d
  Steps:
    1, renderer.cpp:263:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:280:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 100, height / 2 + 170);
                       ^
  Report hash: 9ef1c474cf894e24195b213770d204aa
  Steps:
    1, renderer.cpp:280:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:280:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 100, height / 2 + 170);
                                        ^
  Report hash: 8292c4025befc4e0451cd07e8c681e09
  Steps:
    1, renderer.cpp:280:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:281:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 100, height / 2 + 170);
                       ^
  Report hash: 1ac3ac7cf025bc2766ad81d794816d03
  Steps:
    1, renderer.cpp:281:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:281:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 100, height / 2 + 170);
                                        ^
  Report hash: ea2775981c4cd5283545c9d94900d1bb
  Steps:
    1, renderer.cpp:281:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:282:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 100, height / 2 + 220);
                       ^
  Report hash: 575204ce563f66eec5162544c9da5dcc
  Steps:
    1, renderer.cpp:282:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:282:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 100, height / 2 + 220);
                                        ^
  Report hash: eeefab8d9cd00116ee95202610b16839
  Steps:
    1, renderer.cpp:282:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:283:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 100, height / 2 + 220);
                       ^
  Report hash: 7e49b6dd8b1a83760a8532035d79a735
  Steps:
    1, renderer.cpp:283:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:283:41: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 100, height / 2 + 220);
                                        ^
  Report hash: 122d87e28b1b292f0e6c8af37c5f07b9
  Steps:
    1, renderer.cpp:283:41: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:292:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 80, height / 2 + 210);
                       ^
  Report hash: 6572aaaba61943ad9f2d7bf99ae2a306
  Steps:
    1, renderer.cpp:292:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:292:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 80, height / 2 + 210);
                                       ^
  Report hash: 3ebf0a239d76fbbf0b2f9a1d4b6e3249
  Steps:
    1, renderer.cpp:292:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:293:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 60, height / 2 + 220);
                       ^
  Report hash: a8402040a612ecf0c7bdb735833dd2fe
  Steps:
    1, renderer.cpp:293:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:293:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 60, height / 2 + 220);
                                       ^
  Report hash: d52ae5d7381894df663bb3827922650b
  Steps:
    1, renderer.cpp:293:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:294:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 40, height / 2 + 210);
                       ^
  Report hash: de0310b47d7bb1d8e1394e267ae6db5d
  Steps:
    1, renderer.cpp:294:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:294:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 40, height / 2 + 210);
                                       ^
  Report hash: 99619edc4bf5d0bdc64478f3e237dfd6
  Steps:
    1, renderer.cpp:294:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:295:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 60, height / 2 + 200);
                       ^
  Report hash: fb3acbe063677aecb33bb13099430d59
  Steps:
    1, renderer.cpp:295:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:295:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 60, height / 2 + 200);
                                       ^
  Report hash: 65a9046781b89d1062623426c93390be
  Steps:
    1, renderer.cpp:295:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:296:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 40, height / 2 + 190);
                       ^
  Report hash: f72716e4cbcd6eede0e23dcc13e0e821
  Steps:
    1, renderer.cpp:296:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:296:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 40, height / 2 + 190);
                                       ^
  Report hash: 79cb3e0bf8919661f7d2a5ae32541532
  Steps:
    1, renderer.cpp:296:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:297:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 60, height / 2 + 180);
                       ^
  Report hash: c6ef2a8ea7e15ca4e766a219a80163e7
  Steps:
    1, renderer.cpp:297:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:297:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 60, height / 2 + 180);
                                       ^
  Report hash: 5caede59fb4df61d0858fbc6739622fa
  Steps:
    1, renderer.cpp:297:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:298:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 80, height / 2 + 190);
                       ^
  Report hash: d95c07b9b4a24b6a648f1fac163a8dd0
  Steps:
    1, renderer.cpp:298:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:298:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 80, height / 2 + 190);
                                       ^
  Report hash: 5b8a9e287b6834c68a879d6d14ef61d0
  Steps:
    1, renderer.cpp:298:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:303:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 20, height / 2 + 220);
                       ^
  Report hash: 98ad5eedf57eb7ca9e44881214c1ac76
  Steps:
    1, renderer.cpp:303:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:303:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 20, height / 2 + 220);
                                       ^
  Report hash: c0175486f6fb2354bc214efc765307d8
  Steps:
    1, renderer.cpp:303:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:304:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 20, height / 2 + 180);
                       ^
  Report hash: 72cb0236ef51c8e3a6b6887aae18ce3c
  Steps:
    1, renderer.cpp:304:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:304:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 20, height / 2 + 180);
                                       ^
  Report hash: 62266e3db6b8bf22666a657468682a54
  Steps:
    1, renderer.cpp:304:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:305:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 35, height / 2 + 220);
                       ^
  Report hash: 39ea4568d1585d0150d2cad97011cf3a
  Steps:
    1, renderer.cpp:305:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:305:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 35, height / 2 + 220);
                                       ^
  Report hash: 75402e31bea82431ca2a88ffce376eaa
  Steps:
    1, renderer.cpp:305:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:306:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 5, height / 2 + 220);
                       ^
  Report hash: 72c70b68253bfdf25de4a0099e588145
  Steps:
    1, renderer.cpp:306:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:306:39: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 - 5, height / 2 + 220);
                                      ^
  Report hash: a34cf67bac80422be9adae4409390672
  Steps:
    1, renderer.cpp:306:39: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:311:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 0, height / 2 + 180);
                       ^
  Report hash: 5d82d0eebdfbe2ce958444c3372277d6
  Steps:
    1, renderer.cpp:311:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:311:39: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 0, height / 2 + 180);
                                      ^
  Report hash: d97a580a8aa8814c565262239b31bb16
  Steps:
    1, renderer.cpp:311:39: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:312:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 15, height / 2 + 220);
                       ^
  Report hash: 8d02902260ed54de518ee4bf73631bb7
  Steps:
    1, renderer.cpp:312:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:312:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 15, height / 2 + 220);
                                       ^
  Report hash: eb5f26f453dd6eb10c7750059e88c54a
  Steps:
    1, renderer.cpp:312:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:313:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 30, height / 2 + 180);
                       ^
  Report hash: ce6e3c756545904aca19b7bd56e8502f
  Steps:
    1, renderer.cpp:313:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:313:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 30, height / 2 + 180);
                                       ^
  Report hash: 744e08f57c59917713891845f8036ba6
  Steps:
    1, renderer.cpp:313:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:316:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 7, height / 2 + 200);
                       ^
  Report hash: e74141ca581147cbfabfda9520aca547
  Steps:
    1, renderer.cpp:316:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:316:39: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 7, height / 2 + 200);
                                      ^
  Report hash: 538245c0a8ce7c234b357ec53b503d1c
  Steps:
    1, renderer.cpp:316:39: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:317:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 23, height / 2 + 200);
                       ^
  Report hash: 0a1843aa2ddd3daca5c93034ee33afd4
  Steps:
    1, renderer.cpp:317:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:317:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 23, height / 2 + 200);
                                       ^
  Report hash: 908376016c4e6a708a3e247addb991a2
  Steps:
    1, renderer.cpp:317:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:322:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 40, height / 2 + 180);
                       ^
  Report hash: c131088ac41f9551ced2791bfb503ee9
  Steps:
    1, renderer.cpp:322:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:322:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 40, height / 2 + 180);
                                       ^
  Report hash: c1f74a7be0f4f3889084acf7d0da0734
  Steps:
    1, renderer.cpp:322:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:323:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 40, height / 2 + 220);
                       ^
  Report hash: 0de9e1df1c19a62f96ff783298922260
  Steps:
    1, renderer.cpp:323:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:323:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 40, height / 2 + 220);
                                       ^
  Report hash: 224fd076e5d438fc4ab6f6206306ebfd
  Steps:
    1, renderer.cpp:323:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:324:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 60, height / 2 + 220);
                       ^
  Report hash: 522d888d5b315f08b2bf9b93142c8410
  Steps:
    1, renderer.cpp:324:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:324:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 60, height / 2 + 220);
                                       ^
  Report hash: e1917260ddc15a8ce562b6707fa41298
  Steps:
    1, renderer.cpp:324:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:325:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 70, height / 2 + 210);
                       ^
  Report hash: f99f2b2c16c8de74c18547a3bb75f6f3
  Steps:
    1, renderer.cpp:325:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:325:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 70, height / 2 + 210);
                                       ^
  Report hash: b2d969bff325b8f63559e91c7f1a019e
  Steps:
    1, renderer.cpp:325:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:326:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 60, height / 2 + 200);
                       ^
  Report hash: 95d35e5d1181f7360afefa23a866bab2
  Steps:
    1, renderer.cpp:326:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:326:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 60, height / 2 + 200);
                                       ^
  Report hash: 862fa2691015ad04d294063a2cea6c55
  Steps:
    1, renderer.cpp:326:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:327:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 40, height / 2 + 200);
                       ^
  Report hash: 1544c3b6c0036913616cf107a9468bab
  Steps:
    1, renderer.cpp:327:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:327:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 40, height / 2 + 200);
                                       ^
  Report hash: c1ba2ecc0918b554f0f053f52e4418a6
  Steps:
    1, renderer.cpp:327:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:330:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 60, height / 2 + 200);
                       ^
  Report hash: 95d35e5d1181f7360afefa23a866bab2
  Steps:
    1, renderer.cpp:330:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:330:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 60, height / 2 + 200);
                                       ^
  Report hash: 862fa2691015ad04d294063a2cea6c55
  Steps:
    1, renderer.cpp:330:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:331:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 70, height / 2 + 180);
                       ^
  Report hash: 18b1cdc09248944509c9c0c7b1a6cc6f
  Steps:
    1, renderer.cpp:331:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:331:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 70, height / 2 + 180);
                                       ^
  Report hash: a0aee178b6f1ed478d6e2c4c8d3b8fd3
  Steps:
    1, renderer.cpp:331:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:336:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 80, height / 2 + 220);
                       ^
  Report hash: ca8e5790d3c0d098cbebbfca82c0ff63
  Steps:
    1, renderer.cpp:336:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:336:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 80, height / 2 + 220);
                                       ^
  Report hash: 848626657553356698cfdf1412924998
  Steps:
    1, renderer.cpp:336:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:337:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 80, height / 2 + 180);
                       ^
  Report hash: 26ae1157b073986cc7548d69bc4fabeb
  Steps:
    1, renderer.cpp:337:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:337:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 80, height / 2 + 180);
                                       ^
  Report hash: ec43645cdf8fe6b0d42c722898f29af0
  Steps:
    1, renderer.cpp:337:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:338:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 65, height / 2 + 220);
                       ^
  Report hash: fe9637dc56d3d0cc78a0a9701189fc2c
  Steps:
    1, renderer.cpp:338:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:338:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 65, height / 2 + 220);
                                       ^
  Report hash: e91b178dea91c62126c2ecde78214389
  Steps:
    1, renderer.cpp:338:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:339:24: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 95, height / 2 + 220);
                       ^
  Report hash: 76d07c912edba3913de802adfd82b2b3
  Steps:
    1, renderer.cpp:339:24: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:339:40: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            glVertex2f(width / 2 + 95, height / 2 + 220);
                                       ^
  Report hash: c3579750434a46d3efc5b605d137c4d7
  Steps:
    1, renderer.cpp:339:40: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:403:24: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluPerspective(45.0f, aspect, 0.1f, 100.0f);
                       ^
  Report hash: 4054b2daa3d1781c189c8a5727db59b9
  Steps:
    1, renderer.cpp:403:24: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:403:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluPerspective(45.0f, aspect, 0.1f, 100.0f);
                              ^
  Report hash: 227c381e58c1b33ee9a786c4fed4502e
  Steps:
    1, renderer.cpp:403:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:403:39: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluPerspective(45.0f, aspect, 0.1f, 100.0f);
                                      ^
  Report hash: f456b0c5a474d0b6aa29a8ff0df17106
  Steps:
    1, renderer.cpp:403:39: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:403:45: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluPerspective(45.0f, aspect, 0.1f, 100.0f);
                                            ^
  Report hash: 71db4e88a4afd3c508ab1925aa62c51b
  Steps:
    1, renderer.cpp:403:45: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:409:19: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluLookAt(0.0f, 2.0f, 5.0f,  // Eye position
                  ^
  Report hash: 3d47e9d45abf0a13bed85dabf72e4abe
  Steps:
    1, renderer.cpp:409:19: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:409:25: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluLookAt(0.0f, 2.0f, 5.0f,  // Eye position
                        ^
  Report hash: f22bde501bf9d3efb7f10f7052a38822
  Steps:
    1, renderer.cpp:409:25: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:409:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
        gluLookAt(0.0f, 2.0f, 5.0f,  // Eye position
                              ^
  Report hash: 7ecb834c1c41d1b45105c9ec3031489b
  Steps:
    1, renderer.cpp:409:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:410:19: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
                  0.0f, 0.0f, 0.0f,  // Look at position
                  ^
  Report hash: e33559159b777e7181dc6b54b1c667e9
  Notes:
    1, renderer.cpp:410:9: ~~~~~~~~~ (fixit)
    2, renderer.cpp:410:19: 0.0f, 0.0f, 0.0f,  // Look at position (fixit)
    3, renderer.cpp:410:19: ^~~~ (fixit)
  Steps:
    1, renderer.cpp:410:19: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:410:25: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
                  0.0f, 0.0f, 0.0f,  // Look at position
                        ^
  Report hash: 3d6c36fc136f8b82d199817a68671573
  Notes:
    1, renderer.cpp:410:9: ~~~~~~~~~ (fixit)
    2, renderer.cpp:410:19: 0.0f, 0.0f, 0.0f,  // Look at position (fixit)
    3, renderer.cpp:410:25: ^~~~ (fixit)
  Steps:
    1, renderer.cpp:410:25: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:410:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
                  0.0f, 0.0f, 0.0f,  // Look at position
                              ^
  Report hash: 9b3a538ce9e82b0bcc38c4edb8fad6ef
  Notes:
    1, renderer.cpp:410:9: ~~~~~~~~~ (fixit)
    2, renderer.cpp:410:19: 0.0f, 0.0f, 0.0f,  // Look at position (fixit)
    3, renderer.cpp:410:31: ^~~~ (fixit)
  Steps:
    1, renderer.cpp:410:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:411:19: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
                  0.0f, 1.0f, 0.0f   // Up vector
                  ^
  Report hash: 77266c4d0e289f6df1b4491a43697aaa
  Notes:
    1, renderer.cpp:411:9: ~~~~~~~~~ (fixit)
    2, renderer.cpp:411:19: 0.0f, 0.0f, 0.0f,  // Look at position (fixit)
    3, renderer.cpp:411:19: 0.0f, 1.0f, 0.0f   // Up vector (fixit)
    4, renderer.cpp:411:19: ^~~~ (fixit)
  Steps:
    1, renderer.cpp:411:19: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:411:25: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
                  0.0f, 1.0f, 0.0f   // Up vector
                        ^
  Report hash: 11edd579e5bcee53d6e1bc91168e0740
  Notes:
    1, renderer.cpp:411:9: ~~~~~~~~~ (fixit)
    2, renderer.cpp:411:19: 0.0f, 0.0f, 0.0f,  // Look at position (fixit)
    3, renderer.cpp:411:19: 0.0f, 1.0f, 0.0f   // Up vector (fixit)
    4, renderer.cpp:411:25: ^~~~ (fixit)
  Steps:
    1, renderer.cpp:411:25: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:411:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double') [clang-diagnostic-double-promotion]
                  0.0f, 1.0f, 0.0f   // Up vector
                              ^
  Report hash: 5cd9d00f1835e869c7591f664bf53525
  Notes:
    1, renderer.cpp:411:9: ~~~~~~~~~ (fixit)
    2, renderer.cpp:411:19: 0.0f, 0.0f, 0.0f,  // Look at position (fixit)
    3, renderer.cpp:411:19: 0.0f, 1.0f, 0.0f   // Up vector (fixit)
    4, renderer.cpp:411:31: ^~~~ (fixit)
  Steps:
    1, renderer.cpp:411:31: implicit conversion increases floating-point precision: 'float' to 'GLdouble' (aka 'double')

[MEDIUM] /sep/src/workbench/renderer.cpp:435:61: implicit conversion turns floating-point number into integer: 'typename __gnu_cxx::__enable_if<__is_integer<unsigned long>::__value, double>::__type' (aka 'double') to 'int' [clang-diagnostic-float-conversion]
            int patternsPerRow = std::sqrt(patterns.size()) + 1;
                                                            ^
  Report hash: d7174fdfda55c40bf7c92bd31bab254c
  Steps:
    1, renderer.cpp:435:61: implicit conversion turns floating-point number into integer: 'typename __gnu_cxx::__enable_if<__is_integer<unsigned long>::__value, double>::__type' (aka 'double') to 'int'

[MEDIUM] /sep/src/workbench/renderer.cpp:577:36: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float lat0 = M_PI * (-0.5f + (float)(i) / latitudes);
                                   ^
  Report hash: 59a38c1ca6feed5d76df05381bd347e2
  Steps:
    1, renderer.cpp:577:36: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:578:36: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float lat1 = M_PI * (-0.5f + (float)(i + 1) / latitudes);
                                   ^
  Report hash: 019a5ad1192889b6c5e2b373fb9b54c3
  Steps:
    1, renderer.cpp:578:36: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:579:20: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float y0 = radius * sin(lat0);
                   ^
  Report hash: 1bc753c598ae6f642c5aaeed21bd9c5f
  Steps:
    1, renderer.cpp:579:20: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:579:33: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float y0 = radius * sin(lat0);
                                ^
  Report hash: 3f7648475efb3f011a8c8267d51e9b05
  Steps:
    1, renderer.cpp:579:33: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:580:20: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float y1 = radius * sin(lat1);
                   ^
  Report hash: 7e300e96682267eb78ff6bd7b58c1047
  Steps:
    1, renderer.cpp:580:20: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:580:33: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float y1 = radius * sin(lat1);
                                ^
  Report hash: 6764e1395a1d571d143fa45efda01e66
  Steps:
    1, renderer.cpp:580:33: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:581:20: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float r0 = radius * cos(lat0);
                   ^
  Report hash: d2de4bb4daa9cadc67c5eb60c49ff029
  Steps:
    1, renderer.cpp:581:20: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:581:33: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float r0 = radius * cos(lat0);
                                ^
  Report hash: 61da378882f50280329527edbbc55ddb
  Steps:
    1, renderer.cpp:581:33: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:582:20: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float r1 = radius * cos(lat1);
                   ^
  Report hash: ce5e3381fdee4719bec9fc0ae0a7af12
  Steps:
    1, renderer.cpp:582:20: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:582:33: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float r1 = radius * cos(lat1);
                                ^
  Report hash: 48cc053be0881b5a7647a007fb3934d3
  Steps:
    1, renderer.cpp:582:33: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:587:36: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            float lng = 2 * M_PI * (float)(j) / longitudes;
                                   ^
  Report hash: ea9004ea490480a4d7fdc8f79130cc80
  Steps:
    1, renderer.cpp:587:36: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:588:27: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            float x = cos(lng);
                          ^
  Report hash: 9ddd328316f27a8a1172c3aaeeaa5006
  Steps:
    1, renderer.cpp:588:27: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:589:27: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            float z = sin(lng);
                          ^
  Report hash: d1ff159c453020c91a60c0f8e0fbf378
  Steps:
    1, renderer.cpp:589:27: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:593:28: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            float ny = sin(lat1);
                           ^
  Report hash: 3e3af2990eea4be4aef41755e57067d0
  Steps:
    1, renderer.cpp:593:28: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:595:49: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            float norm = sqrt(nx * nx + ny * ny + nz * nz);
                                                ^
  Report hash: ce5cbe05d048e54070d439c765d63894
  Steps:
    1, renderer.cpp:595:49: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:602:22: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            ny = sin(lat0);
                     ^
  Report hash: 059d33c550fb48a3708585a317b9a240
  Steps:
    1, renderer.cpp:602:22: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:604:43: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            norm = sqrt(nx * nx + ny * ny + nz * nz);
                                          ^
  Report hash: fbd53af181881193f0e594a0a3ecd4fa
  Steps:
    1, renderer.cpp:604:43: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:688:75: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
                                                                          ^
  Report hash: 90209278c4d3c92e1c76038487f2cd6b
  Steps:
    1, renderer.cpp:688:75: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:742:75: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        float length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
                                                                          ^
  Report hash: 90209278c4d3c92e1c76038487f2cd6b
  Steps:
    1, renderer.cpp:742:75: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/workbench/renderer.cpp:808:18: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
    return (x >= width / 2 - 100 && x <= width / 2 + 100 && y >= height / 2 + 170 &&
                 ^
  Report hash: b6f2b8a6a0c9272478af826db4ec9fbd
  Steps:
    1, renderer.cpp:808:18: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:808:42: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
    return (x >= width / 2 - 100 && x <= width / 2 + 100 && y >= height / 2 + 170 &&
                                         ^
  Report hash: b8792fee0c59edc1e1b95e196b120b26
  Steps:
    1, renderer.cpp:808:42: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:808:66: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
    return (x >= width / 2 - 100 && x <= width / 2 + 100 && y >= height / 2 + 170 &&
                                                                 ^
  Report hash: 4712e161bb00ff34dd2078e0566ecfa1
  Steps:
    1, renderer.cpp:808:66: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:809:18: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
            y <= height / 2 + 220);
                 ^
  Report hash: 648649f450942e27507c22172f59fa28
  Steps:
    1, renderer.cpp:809:18: result of integer division used in a floating point context; possible loss of precision

Found 125 defect(s) in renderer.cpp

[MEDIUM] /sep/extern/cycles/third_party/sky/include/sky_model.h:302:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SKY_MODEL_H__
        ^
  Report hash: 6ea65871ef55c3707af51f611f8b594c
  Steps:
    1, sky_model.h:302:9: macro name is a reserved identifier

Found 1 defect(s) in sky_model.h

[MEDIUM] /sep/extern/cycles/third_party/sky/source/sky_float3.h:18:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SKY_FLOAT3_H__
        ^
  Report hash: 7ae4fd8b564fc0c9d53b217507a6bcc6
  Steps:
    1, sky_float3.h:18:9: macro name is a reserved identifier

Found 1 defect(s) in sky_float3.h

[MEDIUM] /sep/extern/cycles/third_party/sky/source/sky_nishita.cpp:34:34: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
static const float step_lambda = (max_wavelength - min_wavelength) / (num_wavelengths - 1);
                                 ^
  Report hash: c394c5df2407ce927e72bcffda90ed11
  Steps:
    1, sky_nishita.cpp:34:34: result of integer division used in a floating point context; possible loss of precision

Found 1 defect(s) in sky_nishita.cpp

[LOW] /usr/include/asio/detail/impl/signal_set_service.ipp:145:12: Call to blocking function 'read' inside of critical section [unix.BlockInCriticalSection]
    while (::read(fd, &signal_number, sizeof(int)) == sizeof(int))
           ^
  Report hash: 4d4048db5a8e07237305e792933e397a
  Steps:
     1, signal_set_service.ipp:145:12: Assuming the condition is true
     2, signal_set_service.ipp:145:12: Entering loop body
     3, signal_set_service.ipp:146:11: Assuming 'signal_number' is >= 0
     4, signal_set_service.ipp:146:33: Assuming 'signal_number' is < max_signal_number
     5, signal_set_service.ipp:147:9: Calling 'signal_set_service::deliver_signal'
     6, signal_set_service.ipp:610:1: Entered call from 'pipe_read_op::do_perform'
     7, signal_set_service.ipp:616:10: Entering loop body
     8, signal_set_service.ipp:621:12: Entering loop body
     9, signal_set_service.ipp:629:27: Entering loop body
    10, signal_set_service.ipp:629:9: Looping back to the head of the loop
    11, signal_set_service.ipp:629:27: Loop body executed 0 times
    12, signal_set_service.ipp:621:5: Looping back to the head of the loop
    13, signal_set_service.ipp:640:5: Calling 'scheduler::post_deferred_completions'
    14, scheduler.ipp:410:1: Entered call from 'signal_set_service::deliver_signal'
    15, scheduler.ipp:416:9: Assuming field 'one_thread_' is false
    16, scheduler.ipp:426:24: Calling constructor for 'scoped_lock'
    17, conditionally_enabled_mutex.hpp:49:5: Entered call from 'scheduler::post_deferred_completions'
    18, conditionally_enabled_mutex.hpp:52:11: Assuming field 'enabled_' is true
    19, conditionally_enabled_mutex.hpp:54:9: Calling 'posix_mutex::lock'
    20, posix_mutex.hpp:49:3: Entered call from constructor for 'scoped_lock'
    21, posix_mutex.hpp:51:11: Entering critical section here
    22, conditionally_enabled_mutex.hpp:54:9: Returning from 'posix_mutex::lock'
    23, scheduler.ipp:426:24: Returning from constructor for 'scoped_lock'
    24, signal_set_service.ipp:640:5: Returning from 'scheduler::post_deferred_completions'
    25, signal_set_service.ipp:616:3: Looping back to the head of the loop
    26, signal_set_service.ipp:147:9: Returning from 'signal_set_service::deliver_signal'
    27, signal_set_service.ipp:145:5: Looping back to the head of the loop
    28, signal_set_service.ipp:145:12: Call to blocking function 'read' inside of critical section

Found 1 defect(s) in signal_set_service.ipp

[LOW] /usr/include/boost/asio/detail/impl/signal_set_service.ipp:146:12: Call to blocking function 'read' inside of critical section [unix.BlockInCriticalSection]
    while (::read(fd, &signal_number, sizeof(int)) == sizeof(int))
           ^
  Report hash: 5770f89bb4da766be512d01ba33e841c
  Steps:
     1, signal_set_service.ipp:146:12: Assuming the condition is true
     2, signal_set_service.ipp:146:12: Entering loop body
     3, signal_set_service.ipp:147:11: Assuming 'signal_number' is >= 0
     4, signal_set_service.ipp:147:33: Assuming 'signal_number' is < max_signal_number
     5, signal_set_service.ipp:148:9: Calling 'signal_set_service::deliver_signal'
     6, signal_set_service.ipp:611:1: Entered call from 'pipe_read_op::do_perform'
     7, signal_set_service.ipp:617:10: Entering loop body
     8, signal_set_service.ipp:622:12: Entering loop body
     9, signal_set_service.ipp:630:27: Entering loop body
    10, signal_set_service.ipp:630:9: Looping back to the head of the loop
    11, signal_set_service.ipp:630:27: Loop body executed 0 times
    12, signal_set_service.ipp:622:5: Looping back to the head of the loop
    13, signal_set_service.ipp:641:5: Calling 'scheduler::post_deferred_completions'
    14, scheduler.ipp:411:1: Entered call from 'signal_set_service::deliver_signal'
    15, scheduler.ipp:417:9: Assuming field 'one_thread_' is false
    16, scheduler.ipp:427:24: Calling constructor for 'scoped_lock'
    17, conditionally_enabled_mutex.hpp:50:5: Entered call from 'scheduler::post_deferred_completions'
    18, conditionally_enabled_mutex.hpp:53:11: Assuming field 'enabled_' is true
    19, conditionally_enabled_mutex.hpp:55:9: Calling 'posix_mutex::lock'
    20, posix_mutex.hpp:50:3: Entered call from constructor for 'scoped_lock'
    21, posix_mutex.hpp:52:11: Entering critical section here
    22, conditionally_enabled_mutex.hpp:55:9: Returning from 'posix_mutex::lock'
    23, scheduler.ipp:427:24: Returning from constructor for 'scoped_lock'
    24, signal_set_service.ipp:641:5: Returning from 'scheduler::post_deferred_completions'
    25, signal_set_service.ipp:617:3: Looping back to the head of the loop
    26, signal_set_service.ipp:148:9: Returning from 'signal_set_service::deliver_signal'
    27, signal_set_service.ipp:146:5: Looping back to the head of the loop
    28, signal_set_service.ipp:146:12: Call to blocking function 'read' inside of critical section

Found 1 defect(s) in signal_set_service.ipp


----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
LOW      |                70
HIGH     |                70
MEDIUM   |               392
----------------------------
----=================----

----==== Checker Statistics ====----
-------------------------------------------------------------------------
Checker name                               | Severity | Number of reports
-------------------------------------------------------------------------
deadcode.DeadStores                        | LOW      |                11
core.CallAndMessage                        | HIGH     |                 3
security.FloatLoopCounter                  | MEDIUM   |                 5
core.NullDereference                       | HIGH     |                 5
core.UndefinedBinaryOperatorResult         | HIGH     |                 2
core.NonNullParamChecker                   | HIGH     |                 1
unix.cstring.BadSizeArg                    | MEDIUM   |                 2
misc-confusable-identifiers                | MEDIUM   |                 2
performance-noexcept-swap                  | MEDIUM   |                 2
bugprone-sizeof-expression                 | HIGH     |                10
bugprone-bitwise-pointer-cast              | MEDIUM   |                 5
bugprone-undefined-memory-manipulation     | MEDIUM   |                64
bugprone-suspicious-memory-comparison      | MEDIUM   |                 6
cert-err33-c                               | MEDIUM   |                65
cert-flp30-c                               | HIGH     |                 5
bugprone-switch-missing-default-case       | LOW      |                23
bugprone-integer-division                  | MEDIUM   |                96
readability-suspicious-call-argument       | LOW      |                 8
bugprone-incorrect-roundings               | HIGH     |                44
bugprone-signed-char-misuse                | MEDIUM   |                10
bugprone-inc-dec-in-conditions             | MEDIUM   |                 4
clang-diagnostic-double-promotion          | MEDIUM   |                55
misc-redundant-expression                  | MEDIUM   |                 1
bugprone-bool-pointer-implicit-conversion  | LOW      |                 1
clang-diagnostic-mismatched-tags           | MEDIUM   |                 2
bugprone-forward-declaration-namespace     | LOW      |                 4
clang-diagnostic-unused-parameter          | MEDIUM   |                 2
unix.BlockInCriticalSection                | LOW      |                 5
performance-move-const-arg                 | MEDIUM   |                 1
misc-header-include-cycle                  | LOW      |                18
clang-diagnostic-reserved-identifier       | MEDIUM   |                44
clang-diagnostic-reserved-macro-identifier | MEDIUM   |                10
clang-diagnostic-unused-private-field      | MEDIUM   |                 9
bugprone-unused-return-value               | MEDIUM   |                 4
bugprone-tagged-union-member-count         | MEDIUM   |                 2
clang-diagnostic-float-conversion          | MEDIUM   |                 1
-------------------------------------------------------------------------
----=================----

----==== File Statistics ====----
-----------------------------------------------
File name                   | Number of reports
-----------------------------------------------
bitfield.inl                |                 3
cetintrin.h                 |                 2
imgui_internal.h            |                13
imgui_tables.cpp            |                13
imgui_demo.cpp              |                62
imstb_truetype.h            |                 5
imgui_draw.cpp              |                35
imgui.cpp                   |                28
parser.h                    |                 2
imstb_textedit.h            |                 1
imgui_widgets.cpp           |                39
cuew.c                      |                 6
hipew.c                     |                 2
imgui.h                     |                39
imgui_impl_glfw.cpp         |                 3
imgui_impl_opengl3.cpp      |                 7
imstb_rectpack.h            |                 1
config_manager_stub.cpp     |                 1
sep_engine_wrapper.h        |                 3
workbench_main.cpp          |                 2
memory.h                    |                 2
engine.cpp                  |                 2
memory_tier_manager.cpp     |                 2
evolution.cpp               |                 3
pipewire_capture.cpp        |                 4
types.h                     |                 1
memory_tier.cpp             |                 1
control.h                   |                 1
core.h                      |                 3
impl-client.h               |                 2
impl-core.h                 |                 1
impl-device.h               |                 1
impl-factory.h              |                 1
impl-link.h                 |                 1
impl-metadata.h             |                 1
impl-node.h                 |                 1
impl-port.h                 |                 1
impl.h                      |                 1
properties.h                |                 2
utils.h                     |                 2
buffer.h                    |                 1
meta.h                      |                 1
format-utils.h              |                 2
buffers.h                   |                 1
port-config.h               |                 1
profile.h                   |                 1
route.h                     |                 1
builder.h                   |                 3
iter.h                      |                 2
cleanup.h                   |                 7
json-core.h                 |                30
string.h                    |                 3
type.h                      |                 5
common.h                    |                 1
http_response.h             |                 1
config.cpp                  |                 4
server.cpp                  |                 2
server.h                    |                 1
pipewire_capture.h          |                 2
annealing_demo.cpp          |                 1
socket_adaptors.h           |                 8
audio_visualizer.cpp        |                 4
audio_visualizer.hpp        |                 5
annealing_sim.cpp           |                 1
audio_visualizer_simple.cpp |                 1
mesh_handler.cpp            |                 2
hash.h                      |                 1
cuew.h                      |                 3
memory_garden.cpp           |                 1
memory_garden.hpp           |                 1
genesis_pattern.cpp         |                 2
hipew.h                     |                 3
renderer.cpp                |               125
sky_model.h                 |                 1
sky_float3.h                |                 1
sky_nishita.cpp             |                 1
signal_set_service.ipp      |                 1
signal_set_service.ipp      |                 1
-----------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 214
Number of analyzer reports                | 532
-----------------------------------------------
----=================----
