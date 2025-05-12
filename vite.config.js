import tailwindcss from "@tailwindcss/vite";
import { defineConfig } from "vite";

export default defineConfig({
	plugins: [tailwindcss(), injectCssAsStyleTag()],
});

function injectCssAsStyleTag() {
	return {
		name: "inject-css-as-style-tags",
		apply: "build",
		transformIndexHtml: {
			order: "post",
			handler: (html, context) => {
				const tags = [];
				for (const output of Object.values(context.bundle).filter((output) =>
					output.fileName.endsWith(".css"),
				)) {
					if (!(output.type === "asset" && typeof output.source === "string")) {
						continue;
					}
					tags.push({
						tag: "style",
						children: output.source,
						injectTo: "head",
					});
					html = html.replaceAll(
						RegExp(
							`<link.*href=".*${output.fileName.replace(/[/\-\\^$*+?.()|[\]{}]/g, "\\$&")}".*\\/?>`,
							"gmi",
						),
						"",
					);
				}
				return { html, tags };
			},
		},
	};
}
