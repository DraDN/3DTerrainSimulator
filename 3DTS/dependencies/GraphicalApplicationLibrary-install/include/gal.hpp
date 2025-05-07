#pragma once

namespace gal {
	class GraphicsApp_SDL_OpenGL;
	struct Event_SDL_OpenGL;
	class WindowSDL;

	namespace renderer_opengl {
		class RendererOpenGL;
		class RenderTarget;
		class Texture;
		class Shader;
		class Camera;
		// class CameraOrthographic;
		// class CameraPerspective;
		class ControllableCamera;
		class Light;
		class Model;
		template <typename T> class Buffer;
		class VAO;
		struct VertexAttribute;
	};

	namespace ui_imgui {
		namespace implementation {
			namespace sdl {
				class UIContext;
			};
		};

		namespace styles {
			void apply_red_button();
			void pop_red_button();

			namespace themes {
				void embrace_the_darkness();
			}
		};
	};
};
