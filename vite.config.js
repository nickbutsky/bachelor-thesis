import tailwindcss from "@tailwindcss/vite";
import { defineConfig } from "vite";

export default defineConfig({
	plugins: [tailwindcss(), injectCssAsStyleTag()],
});

function escapeRegex(string) {
	return string.replace(/[/\-\\^$*+?.()|[\]{}]/g, "\\$&");
}

function injectCssAsStyleTag() {
	return {
		name: "inject-css-as-style-tags",
		apply: "build",
		transformIndexHtml: {
			order: "post",
			handler: (html, context) => {
				const tags = [];
				const output = Object.values(context.bundle).filter((output) =>
					output.fileName.endsWith(".css"),
				)[0];
				let outputHtml = html;
				if (output.type === "asset" && typeof output.source === "string") {
					tags.push({
						tag: "style",
						children: output.source,
						injectTo: "head",
					});
					const fileNameRegExp = RegExp(
						`<link.*href=".*${escapeRegex(output.fileName)}".*\\/?>`,
						"gmi",
					);
					outputHtml = html.replaceAll(fileNameRegExp, "");
				}
				return { outputHtml, tags };
			},
		},
	};
}
