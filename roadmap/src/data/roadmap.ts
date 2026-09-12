export type WorkKind = "feature" | "test" | "repair" | "integration";

export type RoadmapTask = {
  id: string;
  title: string;
  detail: string;
  kind: WorkKind;
  files: string[];
  doneByDefault?: boolean;
};

export type Milestone = {
  id: string;
  title: string;
  subtitle: string;
  outcome: string;
  size: "S" | "M" | "L";
  dependencies: string[];
  buildOn: string[];
  tasks: RoadmapTask[];
  acceptance: string[];
  guardrail: string;
};

export const northStar = {
  title: "The scene comes back",
  statement:
    "Load a project, open a scene, render one textured 3D object, move the editor camera, close Psych, and reopen into the same scene.",
  proof: [
    "A real project—not an arbitrary folder—is active.",
    "The viewport shows a textured procedural cube.",
    "WASD + mouse movement only affect a focused viewport.",
    "Scene and project state survive a clean restart.",
    "Missing or corrupt files return to the project browser without crashing.",
  ],
};

export const milestones: Milestone[] = [
  {
    id: "g0",
    title: "Trust the loop",
    subtitle: "Protect the baseline",
    outcome: "Every feature starts from a repeatable Ninja build and a test suite you can trust.",
    size: "S",
    dependencies: [],
    buildOn: ["CMake presets", "GoogleTest", "Headless renderer path"],
    guardrail: "Timebox this. Fix the loop; do not redesign the build system.",
    tasks: [
      {
        id: "g0-baseline",
        title: "Lock the current compiling baseline",
        detail: "Editor, App, and test targets configure and link from a clean Ninja tree.",
        kind: "repair",
        files: ["CMakePresets.json", "Psych/CMakeLists.txt"],
        doneByDefault: true,
      },
      {
        id: "g0-test-isolation",
        title: "Repair filesystem test isolation",
        detail: "A test currently shuts down the global Filesystem and leaves later tests dependent on execution order.",
        kind: "test",
        files: ["tests/filesystem/FilesystemTests.cpp"],
      },
      {
        id: "g0-ci",
        title: "Make CI use the real presets",
        detail: "Replace stale GE_* flags and the X11/Wayland mismatch with the same editor-debug and tests-debug commands used locally.",
        kind: "integration",
        files: [".gitea/workflows/ci.yml", "CMakePresets.json"],
      },
    ],
    acceptance: [
      "cmake --preset editor-debug && cmake --build --preset editor-debug succeeds.",
      "cmake --preset tests-debug && cmake --build --preset tests-debug && ctest --preset tests-debug succeeds twice in a row.",
      "CI runs those same paths on dev changes.",
    ],
  },
  {
    id: "g1",
    title: "Own a real project session",
    subtitle: "Folder selection becomes project state",
    outcome: "Creating or opening a project establishes one authoritative active project and asset root.",
    size: "M",
    dependencies: ["g0"],
    buildOn: ["Project XML round-trip", "ProjectManager", "Project browser", "macOS folder picker"],
    guardrail: "Keep project state in an editor session/service. Panels should request actions, not own the project.",
    tasks: [
      {
        id: "g1-contract",
        title: "Separate CreateProject from OpenProject",
        detail: "Opening must reject folders without valid metadata; creating may make project.xml and the asset directory.",
        kind: "feature",
        files: ["Psych/src/Project/Project.cpp", "Psych/src/Project/ProjectManager.cpp"],
      },
      {
        id: "g1-owner",
        title: "Give the editor one project-session owner",
        detail: "Wire ProjectManager into EditorLayer or a small EditorSession instead of only building a FileNode tree.",
        kind: "integration",
        files: ["Editor/src/EditorLayer.cpp", "Psych/src/Project/ProjectManager.h"],
      },
      {
        id: "g1-paths",
        title: "Establish project-relative paths",
        detail: "Use the active project's asset root for this slice. Remove unused resolver arguments or add a real project binding—do not leave two path authorities.",
        kind: "repair",
        files: ["Psych/src/FileSystem/PathResolver.cpp", "Psych/src/Project/ProjectManager.cpp"],
      },
      {
        id: "g1-file-tree",
        title: "Point the file tree at project assets",
        detail: "After a successful open, display Project::GetAssetRootPath() rather than the entire selected folder.",
        kind: "integration",
        files: ["Editor/src/EditorLayer.cpp", "Editor/src/UI/Modules/MainFileTree.cpp"],
      },
      {
        id: "g1-recents",
        title: "Persist recent and last project",
        detail: "Add mutation/save APIs, de-duplicate successful opens, and remove placeholder /Documents and /Desktop defaults.",
        kind: "feature",
        files: ["Psych/src/Config/DefaultConfig.h", "Psych/src/Config/PsychEngineConfig.cpp"],
      },
      {
        id: "g1-tests",
        title: "Prove project create/open round-trips",
        detail: "Use temporary directories; cover valid create, valid reopen, missing metadata, and corrupt metadata.",
        kind: "test",
        files: ["tests/project/ProjectTests.cpp", "tests/CMakeLists.txt"],
      },
    ],
    acceptance: [
      "Create produces project.xml plus the configured asset directory.",
      "Open establishes exactly one active project and rejects an arbitrary folder.",
      "The project appears in Recents after restarting the editor.",
    ],
  },
  {
    id: "g2",
    title: "Render one textured cube",
    subtitle: "The first visible engine proof",
    outcome: "A deterministic procedural cube with one project-relative PNG renders inside the existing editor viewport.",
    size: "L",
    dependencies: ["g1"],
    buildOn: ["OpenGL 4.1 context", "Shader uniforms", "VBO/IBO/VAO", "Framebuffer viewport", "stb_image vendor"],
    guardrail: "Use a procedural cube and OpenGL only. Assimp, model import, and a general material system wait for the next goal.",
    tasks: [
      {
        id: "g2-index-buffer",
        title: "Fix index-buffer byte sizing",
        detail: "Upload count × sizeof(uint32_t), not count × sizeof(pointer), and make GL resource ownership non-copyable.",
        kind: "repair",
        files: ["Psych/src/Renderer/Platform/Opengl/OpenglBuffer.cpp", "Psych/src/Renderer/Buffer.h"],
      },
      {
        id: "g2-vertex-format",
        title: "Add a position / normal / UV format",
        detail: "VertexPNT already exists; expose a matching format descriptor and OpenGL attribute layout.",
        kind: "feature",
        files: ["Psych/src/Renderer/VertexTypes.h", "Psych/src/Renderer/VertexFormat.cpp"],
      },
      {
        id: "g2-texture",
        title: "Finish Texture2D",
        detail: "Load with stb_image, upload RGBA data, bind by slot, release the GL texture, and return useful load errors.",
        kind: "feature",
        files: ["Psych/src/Renderer/Texture.cpp", "Psych/src/Renderer/Platform/Opengl/OpenglTexture.cpp"],
      },
      {
        id: "g2-cmake",
        title: "Compile the dormant texture sources",
        detail: "Add Texture.cpp, OpenglTexture.cpp, and stb_image_impl.cpp to the engine target.",
        kind: "integration",
        files: ["Psych/CMakeLists.txt"],
      },
      {
        id: "g2-draw-contract",
        title: "Make one honest renderer submission path",
        detail: "Bind shader, texture, vertex array, and transform through Renderer rather than creating a second RendererAPI in a layer.",
        kind: "feature",
        files: ["Psych/src/Renderer/Renderer.cpp", "Psych/src/Renderer/Mesh.h"],
      },
      {
        id: "g2-cube",
        title: "Create the procedural cube and texture shader",
        detail: "Keep the first mesh local and explicit: 24 vertices, 36 indices, one sampler, one MVP matrix, one checker PNG.",
        kind: "feature",
        files: ["Editor/src/EditorLayer.cpp", "Editor/Assets/Shaders/"],
      },
      {
        id: "g2-smoke",
        title: "Add a repeatable GPU smoke check",
        detail: "Verify viewport resize, depth testing, texture orientation, and no high-severity OpenGL messages.",
        kind: "test",
        files: ["tests/renderer/", "Editor/src/UI/Modules/Viewport.cpp"],
      },
    ],
    acceptance: [
      "A textured cube is visible after opening a project.",
      "The cube remains correctly proportioned when the viewport resizes.",
      "A missing PNG shows a fallback texture or clear error—not an invisible object or crash.",
    ],
  },
  {
    id: "g3",
    title: "Move an editor camera",
    subtitle: "Input reaches the thing that owns it",
    outcome: "A frame-rate-independent perspective camera moves only while the viewport owns keyboard and mouse input.",
    size: "M",
    dependencies: ["g2"],
    buildOn: ["Queued GLFW events", "EventDispatcher", "Time::DeltaTime", "ImGui viewport panel"],
    guardrail: "Build one editor camera/controller. Do not create a general input mapping system yet.",
    tasks: [
      {
        id: "g3-dispatch",
        title: "Dispatch events to layers",
        detail: "Walk layers in reverse order until handled before letting input disappear into Window::HandleEvents.",
        kind: "repair",
        files: ["Psych/src/Core/PsychEngine.cpp", "Psych/src/Layers/LayerStack.h"],
      },
      {
        id: "g3-delta",
        title: "Pass real delta time",
        detail: "Call OnUpdate(Time::DeltaTime()) instead of relying on the current default value of 1.",
        kind: "repair",
        files: ["Psych/src/Core/PsychEngine.cpp", "Psych/src/Util/Time.h"],
      },
      {
        id: "g3-camera-math",
        title: "Implement renderer-independent camera math",
        detail: "Own position, yaw/pitch, view, perspective projection, near/far planes, and viewport aspect.",
        kind: "feature",
        files: ["Psych/src/Renderer/Camera.h", "Psych/src/Renderer/Camera.cpp"],
      },
      {
        id: "g3-viewport-state",
        title: "Expose viewport ownership state",
        detail: "Track focus, hover, and bounds so controls know when the viewport—not another panel—owns input.",
        kind: "feature",
        files: ["Editor/src/UI/Modules/Viewport.h", "Editor/src/UI/Modules/Viewport.cpp"],
      },
      {
        id: "g3-controller",
        title: "Add focused WASD + RMB mouse look",
        detail: "Respect ImGui capture, clamp pitch, scale motion by delta time, and update projection on resize.",
        kind: "feature",
        files: ["Editor/src/EditorLayer.cpp", "Psych/src/Events/KeyEvent.h", "Psych/src/Events/MouseEvents.h"],
      },
      {
        id: "g3-tests",
        title: "Test camera math and input gating",
        detail: "Keep matrix/controller tests headless; reserve one short manual check for native mouse capture.",
        kind: "test",
        files: ["tests/renderer/CameraTests.cpp"],
      },
    ],
    acceptance: [
      "Movement speed is stable across frame rates.",
      "Typing or dragging in another panel never moves the camera.",
      "The projection aspect follows the framebuffer-backed viewport.",
    ],
  },
  {
    id: "g4",
    title: "Open a minimal scene document",
    subtitle: "Persist data before architecture",
    outcome: "A small .psychscene file is the source of truth for the cube instead of hard-coded EditorLayer state.",
    size: "M",
    dependencies: ["g1", "g2"],
    buildOn: ["UUID", "Cereal XML", "Project-relative asset root", "Working cube resources"],
    guardrail: "Use serializable records in a vector. A full ECS, prefab system, and generic serializer are unnecessary for this proof.",
    tasks: [
      {
        id: "g4-model",
        title: "Define the smallest scene model",
        detail: "Scene ID/name plus entity ID/name, transform, primitive identifier, and project-relative texture reference.",
        kind: "feature",
        files: ["Psych/src/Scene/Scene.h", "Psych/src/Scene/SceneEntity.h"],
      },
      {
        id: "g4-serializer",
        title: "Round-trip one .psychscene format",
        detail: "Use Cereal directly for the slice and return explicit errors for missing, malformed, and unsupported documents.",
        kind: "feature",
        files: ["Psych/src/Scene/SceneSerializer.cpp", "Psych/src/Errors/Errors.h"],
      },
      {
        id: "g4-active-scene",
        title: "Give the editor one active scene",
        detail: "Open into a temporary scene first; replace the active scene only after parsing and resource creation succeed.",
        kind: "integration",
        files: ["Editor/src/EditorLayer.h", "Editor/src/EditorLayer.cpp"],
      },
      {
        id: "g4-file-dispatch",
        title: "Route scene files by type",
        detail: "Double-clicking a .psychscene should open it; text files can continue to use FileViewerPanel.",
        kind: "integration",
        files: ["Editor/src/UI/Modules/MainFileTree.cpp", "Editor/src/EditorLayer.cpp"],
      },
      {
        id: "g4-save",
        title: "Add scene dirty state and Save",
        detail: "Replace the demo menu item's empty callback with an editor command that saves the active scene.",
        kind: "feature",
        files: ["Editor/src/UI/Modules/EditorMenuBar.cpp", "Psych/src/Scene/Scene.h"],
      },
      {
        id: "g4-tests",
        title: "Prove scene round-trip and safe failure",
        detail: "Preserve IDs, transform, primitive, and texture path; a corrupt file must not destroy the active scene.",
        kind: "test",
        files: ["tests/scene/SceneSerializerTests.cpp"],
      },
    ],
    acceptance: [
      "Opening a scene reconstructs the textured cube from serialized data.",
      "Save then reload reproduces every scene field.",
      "A failed load leaves the previous scene alive and reports a useful error.",
    ],
  },
  {
    id: "g5",
    title: "Complete the restart loop",
    subtitle: "The vertical slice closes",
    outcome: "A clean shutdown persists the active scene and the next launch restores it—or safely falls back to the browser.",
    size: "M",
    dependencies: ["g3", "g4"],
    buildOn: ["Active project", "Active scene", "Editor camera", "Ordered layer teardown"],
    guardrail: "Restore only one project, one scene, and optionally one editor-camera pose. Multi-document recovery comes later.",
    tasks: [
      {
        id: "g5-last-scene",
        title: "Store the startup scene in project metadata",
        detail: "Add a project-relative LastScene field and update it only after a scene opens successfully.",
        kind: "feature",
        files: ["Psych/src/Project/DefaultProjectConfig.h", "Psych/src/Project/Project.cpp"],
      },
      {
        id: "g5-save-order",
        title: "Define shutdown save order",
        detail: "Save dirty scene first, then project metadata, then engine recents while filesystems are still alive.",
        kind: "integration",
        files: ["Editor/src/EditorLayer.cpp", "Psych/src/Core/PsychEngine.cpp"],
      },
      {
        id: "g5-restore",
        title: "Restore last valid project and scene",
        detail: "Attempt restore after editor-session creation; only hide the browser after both project and configured scene are valid.",
        kind: "feature",
        files: ["Editor/src/EditorLayer.cpp", "Psych/src/Config/PsychEngineConfig.cpp"],
      },
      {
        id: "g5-camera-pose",
        title: "Choose and persist editor-camera pose",
        detail: "For the strongest proof, store it as editor scene metadata. If omitted, document that only scene content is restored.",
        kind: "feature",
        files: ["Psych/src/Scene/Scene.h", "Editor/src/EditorLayer.cpp"],
      },
      {
        id: "g5-fallback",
        title: "Make recovery boring",
        detail: "Missing project, scene, or texture data should show an error and return to a usable project browser.",
        kind: "repair",
        files: ["Editor/src/UI/Modules/ProjectWindow.cpp", "Editor/src/EditorLayer.cpp"],
      },
      {
        id: "g5-reopen-test",
        title: "Automate the session round-trip",
        detail: "Create, open, mutate, save, destroy the session, recreate it, and assert the same project/scene/camera state.",
        kind: "test",
        files: ["tests/editor/EditorSessionTests.cpp"],
      },
    ],
    acceptance: [
      "Launch → project → scene → textured cube → move camera → close → reopen succeeds.",
      "The same project and scene content return; camera pose returns if included in the chosen contract.",
      "Deleting or corrupting a remembered file falls back to the browser without a crash.",
    ],
  },
];

export const capabilitySnapshot = {
  working: [
    "Engine, window, render-loop, and ordered layer teardown",
    "Framebuffer-backed ImGui viewport with docking and resize",
    "OpenGL shader uniforms plus basic VBO / IBO / VAO drawing",
    "Project XML primitives, folder picker, panel manager, and callbacks",
  ],
  disconnected: [
    "Editor project selection bypasses the existing ProjectManager",
    "Recent projects are read once and never updated",
    "Asset types and handles exist, but the asset manager is not usable",
    "Renderer::SubmitMesh queues no drawing work",
  ],
  missing: [
    "Scene/entity/transform model and scene serialization",
    "Working texture upload/bind/lifetime path",
    "Camera implementation and viewport-scoped controller",
    "Layer event delivery, real delta-time updates, and restart restoration",
  ],
};

export const afterSlice = [
  {
    id: "g6",
    title: "Import a real glTF model",
    dependsOn: "G5",
    proof: "Drop a glTF into project assets, import it once, and instantiate its mesh plus textures in the scene.",
    capabilities: ["Assimp import boundary", "Stable asset registry", "Mesh/material extraction", "Import cache + metadata"],
  },
  {
    id: "g7",
    title: "Edit the scene visually",
    dependsOn: "G6",
    proof: "Select the model, edit its transform with a gizmo, undo it, save, and reopen the edited result.",
    capabilities: ["Hierarchy panel", "Inspector", "Selection model", "Gizmos", "Undo/redo command stack"],
  },
  {
    id: "g8",
    title: "Play the scene",
    dependsOn: "G7",
    proof: "Enter Play, control a runtime camera, stop, and return to the unchanged editor scene.",
    capabilities: ["Edit/play scene copies", "Runtime camera", "Input contexts", "Deterministic state transition"],
  },
  {
    id: "g9",
    title: "Ship one tiny demo",
    dependsOn: "G8",
    proof: "Build a standalone app that opens a cooked project and renders the scene without editor-only files.",
    capabilities: ["Asset cooking", "Runtime project format", "Packaging", "Release smoke test"],
  },
];

export const deferredUntilAfterSlice = [
  "Full ECS",
  "FBX / glTF import",
  "General material graph",
  "Async asset database",
  "Multiple scenes or viewports",
  "Undo / redo",
  "Vulkan and Metal backends",
  "Production packaging",
];

export const defaultCompletedTaskIds = milestones.flatMap((milestone) =>
  milestone.tasks.filter((task) => task.doneByDefault).map((task) => task.id),
);
